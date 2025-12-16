/* Windows compatibility header for sys/types.h
 * Provides Unix type definitions for Windows/MSVC
 */

#ifndef _WIN32_COMPAT_SYS_TYPES_H
#define _WIN32_COMPAT_SYS_TYPES_H

#ifdef _WIN32

#include <stddef.h>
#include <stdint.h>

/* Basic type definitions */
#ifndef _PID_T_DEFINED
#define _PID_T_DEFINED
typedef int pid_t;
#endif

#ifndef _UID_T_DEFINED
#define _UID_T_DEFINED
typedef int uid_t;
#endif

#ifndef _GID_T_DEFINED
#define _GID_T_DEFINED
typedef int gid_t;
#endif

#ifndef _MODE_T_DEFINED
#define _MODE_T_DEFINED
typedef unsigned short mode_t;
#endif

/* File offset type - use 64-bit on Windows */
#ifndef _OFF_T_DEFINED
#define _OFF_T_DEFINED
typedef __int64 off_t;
#endif

/* Size type */
#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef ptrdiff_t ssize_t;
#endif

/* Time types */
#ifndef _TIME_T_DEFINED
#define _TIME_T_DEFINED
typedef __time64_t time_t;
#endif

/* Device type */
#ifndef _DEV_T_DEFINED
#define _DEV_T_DEFINED
typedef unsigned int dev_t;
#endif

/* Inode type */
#ifndef _INO_T_DEFINED
#define _INO_T_DEFINED
typedef unsigned long ino_t;
#endif

/* Network types */
#ifndef _SOCKLEN_T_DEFINED
#define _SOCKLEN_T_DEFINED
typedef int socklen_t;
#endif

/* Additional types that might be needed */
#ifndef _CADDR_T_DEFINED
#define _CADDR_T_DEFINED
typedef char *caddr_t;
#endif

/* Unsigned short (commonly used) */
#ifndef _USHORT_DEFINED
#define _USHORT_DEFINED
typedef unsigned short ushort;
#endif

/* Unsigned char */
#ifndef _UCHAR_DEFINED
#define _UCHAR_DEFINED
typedef unsigned char uchar;
#endif

/* Unsigned int */
#ifndef _UINT_DEFINED
#define _UINT_DEFINED
typedef unsigned int uint;
#endif

/* Unsigned long */
#ifndef _ULONG_DEFINED
#define _ULONG_DEFINED
typedef unsigned long ulong;
#endif

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_SYS_TYPES_H */

