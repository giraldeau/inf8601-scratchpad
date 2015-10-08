#include <QDebug>
#include <QString>
#include <cstdio>
#include <cassert>

#include "ops.h"
#include "vec.h"
#include <xmmintrin.h>

__attribute__((noinline))
float fadd_cpp(float x, float y)
{
    return x + y;
}

__attribute__((noinline))
void array_addss_cpp(float *vector, float scalar, int length)
{
    /* TODO */
}

__attribute__((noinline))
void array_addss_cpp_vect(float *vector, float scalar, int length)
{
    /* TODO */
}

void assembly_ops()
{
    hello();

    std::printf("%lX\n", add2(1, 2));

    long cafebabe1 = add10_v1(0xCAFE0000, 0, 0, 0, 0, 0, 0, 0, 0, 0x0000BABE);
    long cafebabe2 = add10_v2(0xCAFE0000, 0, 0, 0, 0, 0, 0, 0, 0, 0x0000BABE);
    std::printf("%lX\n", cafebabe1);
    std::printf("%lX\n", cafebabe2);

    std::printf("%ld\n", min3_v1(3, 10, -12));
    std::printf("%ld\n", max3_v1(3, 10, -12));

    std::printf("%ld\n", eq2(1, 2));
    std::printf("%ld\n", eq2(2, 2));

    std::printf("%f\n", fadd_cpp(1.1, 1.1));
    std::printf("%f\n", fadd2(1.1, 1.1));
}

void reset(QVector<float> &vec)
{
    for (int i = 0; i < vec.size(); i++)
        vec[i] = 0.0;
}

#define ROUND_DOWN(x, s) ((x) & ~((s)-1))
void assembly_vec()
{
    long n = 10;
    QVector<float> data(n);
    float val = -8346975.0; // 0xcafebabe

    movss_ex1();
    movups_ex1();

    reset(data);
    qDebug() << "orig" << data;

    reset(data);
    array_addss_cpp(data.data(), val, data.size());
    qDebug() << "cpp " << data;

    reset(data);
    array_addss_iter(data.data(), val, data.size());
    qDebug() << "asmi" << data;

    reset(data);
    array_addss_vect(data.data(), val, data.size());
    qDebug() << "asmv" << data;

    reset(data);
    array_addss_cpp_vect(data.data(), val, data.size());
    qDebug() << "cppv" << data;

}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    assembly_ops();
    assembly_vec();

    return 0;
}
