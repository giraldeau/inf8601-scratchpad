#include <QDebug>
#include <cassert>

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    int n = 1000000;
    int cnt = 0;

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        #pragma omp atomic
        cnt++;
    }

    qDebug() << cnt;

    assert(cnt == n);

    return 0;
}
