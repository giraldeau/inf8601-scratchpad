#include <QDebug>

__attribute__((noinline))
void array_add(float *a, float *b, float *c, long n)
{
    /* TODO */
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    long n = 1024;
    float a[n];
    float b[n];
    float c[n];

    // reset data
    for (int i = 0; i < n; i++) {
        a[i] = 0;
        b[i] = i;
        c[i] = 1;
    }

    array_add(a, b, c, n);

    qDebug() << a[0] << a[1] << a[2];
    return 0;
}


/*
for (long i = 0; i < n; i++) {
    a[i] = b[i] + c[i];
    if (a[i] == 0)
        break;
}

for (int i = 0; b[i] != 0; i++) {
    a[i] = b[i] + c[i];
}

for (int i = 0; i < n - 1; i++) {
    a[i] = a[i + 1] + b[i];
}

for (int i = 0; i < n; i++) {
    a[i] = b[i] + c[i];
}

for (int i = 0; i < n; i++) {
    a[i] = b[i] + c[i];
    for (int j = 0; j < n; j++){
        x[i][j] = y[i][j] * a[i];
    }
}

for (int i = 0; i < 8; i++) {
    a[i] = b[i] * x;
}

for (int i = 0; i < 2; i+=4) {
    a[i + 0] = b[i + 0] * x;
    a[i + 1] = b[i + 1] * x;
    a[i + 2] = b[i + 2] * x;
    a[i + 3] = b[i + 3] * x;
}
*/
