#include <QDebug>
#include <QVector>
#include <omp.h>
#include <cassert>

void check(QVector<int> &v, int factor, int act)
{
    int exp = 0;
    for (int i = 0; i < v.length(); i++) {
        assert(v[i] == i * factor);
        exp += v[i];
    }
    assert(act == exp);
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    // variables
    int n = 10000;
    int factor = 42;
    int sum = 0;
    int i = 0;
    QVector<int> v(n);

    for (i = 0; i < n; i++) {
        v[i] = i * factor;
        sum += v[i];
    }

    check(v, factor, sum);
    return 0;
}
