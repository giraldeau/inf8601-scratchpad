#include <QDebug>
#include <mpi.h>

int main(int argc, char **argv)
{
    int numprocs, rank;
    MPI_Status status;
    MPI_Comm comm1d;

    int max = (1 << 20);
    QVector<int> send(max);
    QVector<int> recv(max);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Finalize();
    qDebug() << "rank" << rank << "done";

    return 0;
}
