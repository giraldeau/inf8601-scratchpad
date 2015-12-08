TEMPLATE = subdirs

SUBDIRS += \
    mpi \
    01-pthread-base \
    02-pthread-stack \
    03-pthread-recursive \
    04-pthread-max \
    05-pthread-pool \
    06-pthread-image \
    07-pthread-rwlock \
    08-pthread-mutex \
    09-pthread-cond-lost \
    09-pthread-cond-wait \
    10-cpp-lambda \
    11-tbb-parallel-for \
    12-tbb-parallel-reduce \
    13-tbb-parallel-sort \
    14-speedup \
    15-parallel_sort_perf \
    16-cache-align \
    17-cache-pmu \
    18-cache-patterns \
    19-omp-base \
    20-omp-speedup \
    21-omp-scope \
    22-omp-seq \
    23-omp-task \
    24-omp-atomic \
    25-simd-base \
    26-simd-autovec \
    27-race \
    28-memcheck \
    29-deadlock \
    30-bitfields \
    31-static-check \
    33-bandwith-flops \
    34-stream \
    35-roofline \
    36-cache-size
