#include <QDebug>
#include <QVector>
#include <QElapsedTimer>
#include <functional>
#include <omp.h>

double time_it(std::function<void ()> fn)
{
    QElapsedTimer timer;

    // warm-up
    fn();

    timer.start();
    fn();
    ulong elapsed = timer.nsecsElapsed();
    return (double) elapsed / 1000000000.0;
}

double speedup(std::function<void ()> serial, std::function<void ()> parallel)
{
    double s = time_it(serial);
    double p = time_it(parallel);
    return s / p;
}

void memset_benchmark()
{
    // init some data
    int n = 1000000;
    QVector<int> v(n);

    auto memset_serial = [&]() {
        for (int i = 0; i < n; i++) {
            v[i] = i * i;
        }
    };

    auto memset_parallel = [&]() {
        for (int i = 0; i < n; i++) {
            v[i] = i * i;
        }
    };

    double memset_speedup = speedup(memset_serial, memset_parallel);
    qDebug() << "memset speedup:" << memset_speedup;
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    memset_benchmark();

    return 0;
}
