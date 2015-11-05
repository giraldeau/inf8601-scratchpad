#include <QDebug>
#include <cassert>
#include <mpi.h>

#define M_PI 3.14159

/*
 * Calculer la valeur de PI en intégrant la fonction f(x) = 4 / (1 + x^2)
 * Source: Using MPI second edition, The MIT Press, 1999
 */

#define N 1000

int main(int argc, char *argv[])
{
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 42;

    /* Le rank 0 (maitre) décide du nombre d'intervalles à calculer */
    if (rank == 0) {
        n = N;
    }

    /* Communiquer la variable du maitre à tous les autres processus */
    qDebug() << "before bcast rank=" << rank << "n=" << n;

    qDebug() << "after bcast  rank=" << rank << "n=" << n;

    assert(n == N);

    /* calculer PI */
    double h = 1.0 / n;
    double sum = 0.0;
    for (int i = rank + 1; i < n; i+= size) {
        double x = h * ((double) i - 0.5);
        sum += (4.0 / (1.0 + x * x));
    }
    double local_pi = h * sum;
    double pi = 0.0;

    /* Rassembler la somme de toutes les variables vers le processus maitre */


    /*
     * Avec MPI_Reduce(), le résultat de la réduction est disponible
     * uniquement dans le processeur maitre spécifié. MPI_Allreduce() fait
     * le même traitement, mais distribue le résultat à tous les processeurs.
     */
    //MPI_Allreduce(&local_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    qDebug() << "rank=" << rank << "local_pi" << local_pi << "pi" << pi;

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        qDebug() << "rapport: pi=" << pi << "% error=" << (M_PI - pi) / M_PI * 100.0;
    }
    MPI_Finalize();
    return 0;
}
