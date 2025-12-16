/*
 * sys/resource.h - Windows compatibility header for Unix resource operations
 * 
 * This header provides stub implementations for resource limit functions
 * that are not available on Windows.
 */

#ifndef _WIN32_COMPAT_SYS_RESOURCE_H
#define _WIN32_COMPAT_SYS_RESOURCE_H

#ifdef _WIN32

#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Resource usage structure */
struct rusage {
    struct timeval ru_utime;    /* user time used */
    struct timeval ru_stime;    /* system time used */
    long ru_maxrss;             /* maximum resident set size */
    long ru_ixrss;              /* integral shared memory size */
    long ru_idrss;              /* integral unshared data size */
    long ru_isrss;              /* integral unshared stack size */
    long ru_minflt;             /* page reclaims */
    long ru_majflt;             /* page faults */
    long ru_nswap;              /* swaps */
    long ru_inblock;            /* block input operations */
    long ru_oublock;            /* block output operations */
    long ru_msgsnd;             /* messages sent */
    long ru_msgrcv;             /* messages received */
    long ru_nsignals;           /* signals received */
    long ru_nvcsw;              /* voluntary context switches */
    long ru_nivcsw;             /* involuntary context switches */
};

/* Resource limit structure */
struct rlimit {
    unsigned long rlim_cur;     /* soft limit */
    unsigned long rlim_max;     /* hard limit */
};

/* Who constants for getrusage */
#define RUSAGE_SELF     0
#define RUSAGE_CHILDREN -1

/* Resource constants for getrlimit/setrlimit */
#define RLIMIT_CPU      0       /* CPU time in seconds */
#define RLIMIT_FSIZE    1       /* Maximum file size */
#define RLIMIT_DATA     2       /* Maximum data segment size */
#define RLIMIT_STACK    3       /* Maximum stack size */
#define RLIMIT_CORE     4       /* Maximum core file size */
#define RLIMIT_NOFILE   5       /* Maximum number of open files */
#define RLIMIT_AS       6       /* Maximum address space */

#define RLIM_INFINITY   (~0UL)

/* Stub implementations */
static inline int getrusage(int who, struct rusage *usage)
{
    if (usage) {
        memset(usage, 0, sizeof(struct rusage));
    }
    return 0;
}

static inline int getrlimit(int resource, struct rlimit *rlim)
{
    if (rlim) {
        rlim->rlim_cur = RLIM_INFINITY;
        rlim->rlim_max = RLIM_INFINITY;
    }
    return 0;
}

static inline int setrlimit(int resource, const struct rlimit *rlim)
{
    /* Windows doesn't support most resource limits */
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_SYS_RESOURCE_H */

