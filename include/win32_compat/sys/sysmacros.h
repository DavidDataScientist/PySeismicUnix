/* Windows compatibility header for sys/sysmacros.h
 * Provides macros for extracting major/minor device numbers
 */

#ifndef _WIN32_COMPAT_SYS_SYSMACROS_H
#define _WIN32_COMPAT_SYS_SYSMACROS_H

#ifdef _WIN32

/* On Windows, device numbers are not used the same way as Unix.
 * We provide stub macros that return 0, as Windows doesn't have
 * the same concept of major/minor device numbers.
 * 
 * Note: The tape detection code in filestat.c that uses these
 * macros will not work on Windows, but that's acceptable since
 * Windows doesn't have /dev/rmt0 or /dev/mt0 anyway.
 */

#ifndef major
#define major(dev)  ((unsigned int)(((dev) >> 8) & 0xff))
#endif

#ifndef minor
#define minor(dev)  ((unsigned int)((dev) & 0xff))
#endif

#ifndef makedev
#define makedev(maj, min)  ((((maj) & 0xff) << 8) | ((min) & 0xff))
#endif

#else
/* On Unix systems, include the real header if available */
#include <sys/sysmacros.h>
#endif

#endif /* _WIN32_COMPAT_SYS_SYSMACROS_H */

