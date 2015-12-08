#include <QDebug>
#include <QString>
#include <QVector>
#include <cstdio>
#include <cassert>
#include "tbb/tbb.h"

int main(int argc, char *argv[])
{



    qDebug() << "nb de fils d execution: " << nb_tbb << " taille_vecteur_simd: " << taille_vecteur_simd;
    for (i=0;i<n;i++)
    {
        qDebug() << "indice: " << i << " valeur de m: " << *(m+i);
    }

}


