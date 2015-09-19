#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>

#include <QDebug>
#include <QElapsedTimer>

#include <tbb/parallel_for.h>
#include <tbb/parallel_sort.h>

int main(int argc, char *argv[])
{
    const int NB_INTS = 1000000;

    std::vector<int> ints(NB_INTS);
    ints.reserve(NB_INTS);
    for (int i = 0; i < NB_INTS; ++i)
    {
        ints[i] = i;
    }
    std::random_shuffle(ints.begin(), ints.end());

    QElapsedTimer timer;

    timer.start();
    std::sort(ints.begin(), ints.end());
    qDebug() << "Serial: " << timer.nsecsElapsed() / 1e9;

    std::random_shuffle(ints.begin(), ints.end());

    timer.restart();
    tbb::parallel_sort(ints.begin(), ints.end());
    qDebug() << "parallel_sort: " << timer.nsecsElapsed() / 1e9;

    return 0;
}
