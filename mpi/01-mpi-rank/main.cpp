#include <QDebug>

#include <mpi.h>

int main(int argc, char *argv[])
{
    int size, rank;

    /* C++ */

    qDebug() << "C++ size=" << size << "rank=" << rank;

    /* C */

    qDebug() << "C   size=" << size << "rank=" << rank;

    return 0;
}
