#include <QDebug>
#include <linux/perf_event.h>
#include <sys/syscall.h>
#include <unistd.h>

/* perf_event_open syscall wrapper */
static long
sys_perf_event_open(struct perf_event_attr *hw_event,
                    pid_t pid, int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

/* gettid syscall wrapper */
static inline pid_t gettid()
{
    return syscall(SYS_gettid);
}

void write_linear(std::vector<int> &v)
{
    for (uint j = 0; j < v.size(); j++) {
        v[j] = j;
    }
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    void *fn = (void *) sys_perf_event_open;
    (void) fn;

    int samples = 10;
    long sum = 0;

    qDebug() << "\n  avg value" << sum / samples;
    return 0;
}
