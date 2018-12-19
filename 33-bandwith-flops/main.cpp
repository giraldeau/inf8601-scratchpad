#include <QDebug>
#include "benchmark.h"
#include <xmmintrin.h>
#include <cmath>

#include "workload.h"

class LoadFloat : public Workload {
public:
    LoadFloat() : Workload("load_scalar", 0, sizeof(float), 1) { }
    void work() {
        QVector<float> &v = m_floats[0];
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float x = v[i];
                    (void) x;
                }
            }
        );
    }
};

class StoreFloat : public Workload {
public:
    StoreFloat() : Workload("store_scalar", 0, sizeof(float), 1) { }
    void work() {
        QVector<float> &v = m_floats[0];
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    v[i] = 42;
                }
            }
        );
    }
};

class CopyFloat : public Workload {
public:
    CopyFloat() : Workload("copy_scalar", 0, sizeof(float) * 2, 2) { }
    void work() {
        QVector<float> &src = m_floats[0];
        QVector<float> &dst = m_floats[1];
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    dst[i] = src[i];
                }
            }
        );
    }
};

class CopyFloatSIMD : public Workload {
public:
    CopyFloatSIMD() : Workload("copy_simd", 0, sizeof(float) * 2, 2) { }
    void work() {
        QVector<float> &src = m_floats[0];
        QVector<float> &dst = m_floats[1];
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    __m128 ps = _mm_loadu_ps(src.data() + (i * 4));
                    _mm_storeu_ps(dst.data() + (i * 4), ps);
                }
            }
        );
    }
};

class LoadFloatSIMD : public Workload {
public:
    LoadFloatSIMD() : Workload("load_simd", 0, sizeof(float), 1) { }
    void work() {
        QVector<float> &v = m_floats[0];
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile __m128 ps = _mm_loadu_ps(v.data() + (i * 4));
                    (void) ps;
                }
            }
        );
    }
};

class StoreFloatSIMD : public Workload {
public:
    StoreFloatSIMD() : Workload("store_simd", 0, sizeof(float), 1) { }
    void work() {
        QVector<float> &v = m_floats[0];
        __m128 ps = _mm_set1_ps(3.1416);
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    _mm_storeu_ps(v.data() + (i * 4), ps);
                }
            }
        );
    }
};

class MulFlops : public Workload {
public:
    MulFlops() : Workload("mul", 1, 0, 0) { }
    void work() {
        float cst = 3.1416;
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float val = i * cst;
                    (void) val;
                }
            }
        );
    }
};

class DivFlops : public Workload {
public:
    DivFlops() : Workload("div", 1, 0, 0) { }
    void work() {
        float cst = 3.1416;
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float val = i / cst;
                    (void) val;
                }
            }
        );
    }
};

class SinFlops : public Workload {
public:
    SinFlops() : Workload("sin", 1, 0, 0) { }
    void work() {
        float cst = 3.1416;
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    volatile float val = std::sin(i + cst);
                    (void) val;
                }
            }
        );
    }
};

class MulFlopsSIMD : public Workload {
public:
    MulFlopsSIMD() : Workload("mul_simd", 4, 0, 0) { }
    void work() {
        __m128 cstv = _mm_set1_ps(3.1416);
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    __m128 local = _mm_set1_ps(1.234);
                    volatile __m128 val = _mm_mul_ps(local, cstv);
                    (void) val;
                    (void) i;
                }
            }
        );
    }
};

class SAXPYScalar : public Workload {
public:
    SAXPYScalar() : Workload("saxpy", 2, sizeof(float) * 3, 2) { }
    void work() {
        QVector<float> &x = m_floats[0];
        QVector<float> &y = m_floats[1];
        float a = 3.1416;
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    y[i] = a * x[i] + y[i];
                }
            }
        );
    }
};

class SAXPYSIMD : public Workload {
public:
    SAXPYSIMD() : Workload("saxpy_simd", 2, sizeof(float) * 3, 2) { }
    void work() {
        QVector<float> &x = m_floats[0];
        QVector<float> &y = m_floats[1];
        __m128 va = _mm_set1_ps(3.1416);
        tbb::parallel_for(tbb::blocked_range<int>(0, m_size / 4),
            [&](tbb::blocked_range<int> &range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    int id = i * 4;
                    __m128 vx = _mm_loadu_ps(x.data() + id);
                    __m128 vy = _mm_loadu_ps(y.data() + id);
                    vx = _mm_mul_ps(va, vx);
                    vy = _mm_add_ps(vx, vy);
                    _mm_storeu_ps(y.data() + id, vy);
                }
            }
        );
    }
};

// workload: matrix multiplication with blocking
// workload: loop fusion

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    QVector<Workload*> items = {
        new LoadFloat(),
        new StoreFloat(),
        new LoadFloatSIMD(),
        new StoreFloatSIMD(),
        new CopyFloat(),
        new CopyFloatSIMD(),
        new MulFlops(),
        new MulFlopsSIMD(),
        new DivFlops(),
        new SinFlops(),
        new SAXPYScalar(),
        new SAXPYSIMD(),
    };

    Benchmark bench;
    bench.setRepeat(10);

    int size = 1 << 20;
    for (Workload *item : items) {
        item->m_size = size;
        bench.run(item);
    }

    bench.csv("ts.csv", bench.m_ts);
    bench.csv("bw.csv", bench.m_bw);
    bench.csv("fp.csv", bench.m_fp);

    return 0;
}
