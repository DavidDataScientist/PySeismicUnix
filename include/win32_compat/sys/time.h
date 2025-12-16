/*
 * sys/time.h - Windows compatibility header for Unix time operations
 */

#ifndef _WIN32_COMPAT_SYS_TIME_H
#define _WIN32_COMPAT_SYS_TIME_H

#ifdef _WIN32

#include <time.h>
#include <winsock2.h>  /* For struct timeval on Windows */

#ifdef __cplusplus
extern "C" {
#endif

/* gettimeofday implementation for Windows */
#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED
/* struct timeval is defined in winsock2.h */
#endif

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

static inline int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    if (tv) {
        FILETIME ft;
        ULARGE_INTEGER uli;
        
        GetSystemTimeAsFileTime(&ft);
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        
        /* Convert from 100-nanosecond intervals since Jan 1, 1601
         * to seconds and microseconds since Jan 1, 1970 */
        uli.QuadPart -= 116444736000000000ULL;
        tv->tv_sec = (long)(uli.QuadPart / 10000000);
        tv->tv_usec = (long)((uli.QuadPart % 10000000) / 10);
    }
    
    if (tz) {
        TIME_ZONE_INFORMATION tzi;
        GetTimeZoneInformation(&tzi);
        tz->tz_minuteswest = tzi.Bias;
        tz->tz_dsttime = 0;
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_SYS_TIME_H */

