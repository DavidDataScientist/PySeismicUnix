/* Windows compatibility header for fcntl.h
 * Provides file control constants and functions for Windows/MSVC
 */

#ifndef _WIN32_COMPAT_FCNTL_H
#define _WIN32_COMPAT_FCNTL_H

#ifdef _WIN32

#include <io.h>          /* For _open, _creat, etc. */
#include <stdarg.h>      /* For va_list */

/* Include our types */
#include "win32_compat/sys/types.h"

/* 
 * Windows _O_* constants - define directly to avoid include order issues
 * Values from Windows SDK fcntl.h
 */
#ifndef _O_RDONLY
#define _O_RDONLY       0x0000  /* Open for reading only */
#endif
#ifndef _O_WRONLY
#define _O_WRONLY       0x0001  /* Open for writing only */
#endif
#ifndef _O_RDWR
#define _O_RDWR         0x0002  /* Open for reading and writing */
#endif
#ifndef _O_APPEND
#define _O_APPEND       0x0008  /* Append on each write */
#endif
#ifndef _O_CREAT
#define _O_CREAT        0x0100  /* Create file if it doesn't exist */
#endif
#ifndef _O_TRUNC
#define _O_TRUNC        0x0200  /* Truncate flag */
#endif
#ifndef _O_EXCL
#define _O_EXCL         0x0400  /* Exclusive use flag */
#endif
#ifndef _O_TEXT
#define _O_TEXT         0x4000  /* Open in text mode */
#endif
#ifndef _O_BINARY
#define _O_BINARY       0x8000  /* Open in binary mode */
#endif
#ifndef _O_NOINHERIT
#define _O_NOINHERIT    0x0080  /* Child process doesn't inherit file */
#endif
#ifndef _O_TEMPORARY
#define _O_TEMPORARY    0x0040  /* Temporary file */
#endif
#ifndef _O_SHORT_LIVED
#define _O_SHORT_LIVED  0x1000  /* Short-lived file */
#endif

/* File open flags - map Unix O_* to Windows _O_* */
#ifndef O_RDONLY
#define O_RDONLY    _O_RDONLY
#endif
#ifndef O_WRONLY
#define O_WRONLY    _O_WRONLY
#endif
#ifndef O_RDWR
#define O_RDWR      _O_RDWR
#endif
#ifndef O_APPEND
#define O_APPEND    _O_APPEND
#endif
#ifndef O_CREAT
#define O_CREAT     _O_CREAT
#endif
#ifndef O_EXCL
#define O_EXCL      _O_EXCL
#endif
#ifndef O_TRUNC
#define O_TRUNC     _O_TRUNC
#endif
#ifndef O_BINARY
#define O_BINARY    _O_BINARY
#endif
#ifndef O_TEXT
#define O_TEXT      _O_TEXT
#endif

/* Additional flags that might be needed */
#ifndef O_NONBLOCK
#define O_NONBLOCK  0x4000       /* Non-blocking I/O (not directly supported) */
#endif

#ifndef O_SYNC
#define O_SYNC      0x8000       /* Synchronous writes (not directly supported) */
#endif

/* File permission modes (for open with O_CREAT) */
#ifndef S_IRUSR
#define S_IRUSR     _S_IREAD     /* Read permission, owner */
#define S_IWUSR     _S_IWRITE    /* Write permission, owner */
#define S_IXUSR     0x0040       /* Execute permission, owner */
#define S_IRGRP     0x0020       /* Read permission, group */
#define S_IWGRP     0x0010       /* Write permission, group */
#define S_IXGRP     0x0008       /* Execute permission, group */
#define S_IROTH     0x0004       /* Read permission, other */
#define S_IWOTH     0x0002       /* Write permission, other */
#define S_IXOTH     0x0001       /* Execute permission, other */

#define S_IRWXU     (S_IRUSR | S_IWUSR | S_IXUSR)  /* Read, write, execute by owner */
#define S_IRWXG     (S_IRGRP | S_IWGRP | S_IXGRP)  /* Read, write, execute by group */
#define S_IRWXO     (S_IROTH | S_IWOTH | S_IXOTH)  /* Read, write, execute by other */
#endif

/* 
 * Function declarations - DO NOT redeclare functions already in Windows SDK
 * open(), creat() are already declared in <io.h> and <fcntl.h>
 * Only declare functions that don't exist in Windows
 */
#ifdef __cplusplus
extern "C" {
#endif

/* fcntl() does not exist in Windows - declare our compatibility version */
int fcntl(int fd, int cmd, ...);

#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_FCNTL_H */
