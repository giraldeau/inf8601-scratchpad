#include <QDebug>
#include <QElapsedTimer>
#include <QThread>
#include <omp.h>

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    /*
     * Example 1: omp parallel
     */
    qDebug() << "omp parallel begin";
    {
        qDebug() << "rank" << omp_get_thread_num() <<
                    "size" << omp_get_num_threads();
    }
    qDebug() << "omp parallel end";

    /*
     * Example 2: omp parallel sections
     */
    qDebug() << "omp parallel sections begin";
    {
        {
            qDebug() << "section 1 rank" << omp_get_thread_num();
        }
        {
            qDebug() << "section 2 rank" << omp_get_thread_num();
        }
        {
            qDebug() << "section 3 rank" << omp_get_thread_num();
        }
    }
    qDebug() << "omp parallel sections end";

    /*
     * Example 3: omp task
     */
    qDebug() << "omp task begin";
    QElapsedTimer timer;
    timer.start();
    {
        qDebug() << "task 1 rank" << omp_get_thread_num();
        QThread::sleep(1);
    }
    {
        qDebug() << "task 2 rank" << omp_get_thread_num();
        QThread::sleep(1);
    }
    qDebug() << "omp task end" << timer.elapsed();

    /*
     * Example 4: omp parallel single
     */
    qDebug() << "omp parallel single begin";
    {
        {
            qDebug() << "rank" << omp_get_thread_num();
        }
    }
    qDebug() << "omp parallel single end";

    /*
     * Example 5: omp parallel master
     */
    qDebug() << "omp parallel master begin";
    {
        {
            qDebug() << "rank" << omp_get_thread_num();
        }
    }
    qDebug() << "omp parallel master end";

}
