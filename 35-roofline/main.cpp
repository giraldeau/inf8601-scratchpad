#include <QDebug>
#include <QElapsedTimer>
#include <QMap>
#include <functional>
#include <iostream>
#include "tbb/tbb.h"
#include <mmintrin.h>

// compute peak FP
// normalize to peak

typedef struct sample {
    double ts;
    double bw;
    double fp;
} sample_t;

sample_t benchmark(std::function<void (int)> fn, int cpus, int op, qint64 flops, qint64 bytes)
{
    sample_t sample;
    int max_repeat = 3;
    double sum = 0;
    QElapsedTimer time;
    tbb::task_scheduler_init sched(cpus);
    for (int repeat = 0; repeat < max_repeat; repeat++) {
        qint64 t1 = time.nsecsElapsed();
        fn(op);
        sum += time.nsecsElapsed() - t1;
    }
    double mean = sum / max_repeat;
    double fp = flops / (mean / 1E9);
    double bw = bytes / (mean / 1E9);
    qDebug() << "cpus=" << cpus << "ops=" << op << "ts=" << mean << "fp=" << fp << "bw=" << bw;
    sample.ts = mean;
    sample.bw = bw;
    sample.fp = fp;
    return sample;
}

void dump_sample(const char *msg, sample_t &s)
{
    qDebug() << QString("%1 fp=%2 bw=%3")
                .arg(msg)
                .arg(s.fp, 8, 'e', 2)
                .arg(s.bw, 8, 'e', 2);
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int nrcpus = tbb::task_scheduler_init::default_num_threads();
    qint64 base_size = (1 << 25); // sizeof(float) * 2^25 = 4 * 32 = 128MB
    qint64 size = base_size;
    QVector<float> v0, v1, v2, v3;

    /*
     * Le kernel roofline nécessite 16 octets de données par itérations.
     * Le nombre ops indique les itérations de calcul requises pour
     * atteindre l'intensité arithmétique désiré.
     */
    QVector<int> nops = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
    QVector<int> ncpus = {1, nrcpus};

    v0.fill(3.1416, size);
    v1.fill(3.1416, size);
    v2.fill(0, size);
    v3.fill(0, size);

    auto roofline = [&](int op) {
        tbb::parallel_for(tbb::blocked_range<int>(0, size / 8),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    // 32 * 4 bytes for each (size / 8) = 16 * size bytes
                    // op * 16 flops for each (size / 8) = op * 2 * size flops
                    int id = i * 8;
                    __m128 W, X, Y, Z;
                    __m128 A = _mm_loadu_ps(v0.data() + id);
                    __m128 B = _mm_loadu_ps(v1.data() + id);
                    __m128 C = _mm_loadu_ps(v0.data() + id + 4);
                    __m128 D = _mm_loadu_ps(v1.data() + id + 4);
                    for (int repeat = 0; repeat < op; repeat++) {
                        W = _mm_mul_ps(A, A);
                        X = _mm_add_ps(B, B);
                        Y = _mm_mul_ps(C, C);
                        Z = _mm_add_ps(D, D);
                    }
                    _mm_storeu_ps(v2.data() + id, W);
                    _mm_storeu_ps(v3.data() + id, X);
                    _mm_storeu_ps(v2.data() + id + 4, Y);
                    _mm_storeu_ps(v3.data() + id + 4, Z);
                }
            }
        );
    };

    __m128 A, B, C, D;
    A = B = C = D = _mm_set1_ps(3.14159);
    auto flops = [&](int op) { (void) op;
        tbb::parallel_for(tbb::blocked_range<int>(0, size / 8),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    // 16 flops for each (size / 8) == 2 * size flops
                    volatile __m128 W = _mm_mul_ps(A, A);
                    volatile __m128 X = _mm_add_ps(B, B);
                    volatile __m128 Y = _mm_mul_ps(C, C);
                    volatile __m128 Z = _mm_add_ps(D, D);
                    (void) W, X, Y, Z;
                }
            }
        );
    };

    auto bandwidth = [&](int op) { (void) op;
        tbb::parallel_for(tbb::blocked_range<int>(0, size / 8),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    // 16 * 8 for each (size / 8) == 16 * size bytes
                    int id = i * 8;
                    __m128 A = _mm_loadu_ps(v0.data() + id);
                    __m128 B = _mm_loadu_ps(v1.data() + id);
                    __m128 C = _mm_loadu_ps(v0.data() + id + 4);
                    __m128 D = _mm_loadu_ps(v1.data() + id + 4);
                    _mm_storeu_ps(v2.data() + id, A);
                    _mm_storeu_ps(v3.data() + id, B);
                    _mm_storeu_ps(v2.data() + id + 4, C);
                    _mm_storeu_ps(v3.data() + id + 4, D);
                }
            }
        );
    };

    sample_t fp_res = benchmark(flops, nrcpus, 0, 2 * size, 0);
    sample_t bw_res = benchmark(bandwidth, nrcpus, 0, 0, 16 * size);

    QMap<int, QMap<int, sample_t> > map;
    for (int cpus : ncpus) {
        for (int ops : nops) {
            map[cpus][ops] = benchmark(roofline, cpus, ops, 2 * size * ops, 16 * size);
        }
    }

    // report
    dump_sample("Peak FP:", fp_res);
    dump_sample("Peak BW:", bw_res);

    for (int cpus: ncpus) {
        std::cout << cpus << ",";
    }
    std::cout << std::endl;
    for (int ops: nops) {
        for (int cpus: ncpus) {
            QString str = QString::number(map[cpus][ops].fp, 'e', 6);
            std::cout << str.toStdString() << ",";
        }
        std::cout << std::endl;
    }

    return 0;
}

