/* Windows compatibility header for unistd.h
 * Provides Unix function declarations and implementations for Windows/MSVC
 */

#ifndef _WIN32_COMPAT_UNISTD_H
#define _WIN32_COMPAT_UNISTD_H

#ifdef _WIN32

#include <io.h>          /* For _read, _write, _close, etc. */
#include <process.h>     /* For _getpid, etc. */
#include <direct.h>      /* For _getcwd, _chdir, etc. */
#include <stdlib.h>      /* For getenv, etc. */
/* Prevent winsock.h from being included (defines SOCKET which conflicts) */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>     /* For Windows API */
#include "win32_compat/sys/types.h"   /* Our compatibility types */

/* File operations - map to Windows equivalents */
/* Only define if not already defined (to avoid redefinition warnings) */
#ifndef _READ_DEFINED
#define read(fd, buf, count)     _read(fd, buf, (unsigned int)(count))
#define _READ_DEFINED
#endif
#ifndef _WRITE_DEFINED
#define write(fd, buf, count)    _write(fd, buf, (unsigned int)(count))
#define _WRITE_DEFINED
#endif
#ifndef _CLOSE_DEFINED
#define close(fd)                _close(fd)
#define _CLOSE_DEFINED
#endif
#ifndef _LSEEK_DEFINED
#define lseek(fd, offset, whence) _lseek(fd, offset, whence)
#define _LSEEK_DEFINED
#endif

/* Process operations */
#ifndef _GETPID_DEFINED
#define getpid()                 _getpid()
#define _GETPID_DEFINED
#endif
#define getppid()                1  /* Not really available on Windows, return 1 */
#define getuid()                 0  /* Not available, return 0 */
#define getgid()                 0  /* Not available, return 0 */
#define geteuid()                0  /* Not available, return 0 */
#define getegid()                0  /* Not available, return 0 */

/* Directory operations */
#ifndef _CHDIR_DEFINED
#define chdir(path)              _chdir(path)
#define _CHDIR_DEFINED
#endif
#ifndef _GETCWD_DEFINED
#define getcwd(buf, size)        _getcwd(buf, (int)(size))
#define _GETCWD_DEFINED
#endif
#ifndef _RMDIR_DEFINED
#define rmdir(path)              _rmdir(path)
#define _RMDIR_DEFINED
#endif

/* File system operations */
#ifndef _UNLINK_DEFINED
#define unlink(path)             _unlink(path)
#define _UNLINK_DEFINED
#endif
#ifndef _ACCESS_DEFINED
#define access(path, mode)        _access(path, mode)
#define _ACCESS_DEFINED
#endif

/* Sleep operations */
#define sleep(seconds)           Sleep((seconds) * 1000)
#define usleep(microseconds)      Sleep((microseconds) / 1000)

/* Standard file descriptors */
#ifndef STDIN_FILENO
#define STDIN_FILENO  0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

/* File access modes for access() */
#ifndef R_OK
#define R_OK    4   /* Test for read permission */
#define W_OK    2   /* Test for write permission */
#define X_OK    1   /* Test for execute permission */
#define F_OK    0   /* Test for existence */
#endif

/* Seek constants */
#ifndef SEEK_SET
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2
#endif

/* 
 * Function declarations - DO NOT redeclare functions already in Windows SDK
 * _read, _write, _close, _lseek, _getpid, _chdir, _getcwd, _rmdir, _unlink,
 * _access, dup, dup2, isatty, getenv are all in Windows headers.
 * Only declare functions that need our compatibility implementation.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* These functions don't exist in Windows - we provide compatibility versions */
pid_t fork(void);  /* Not really possible on Windows - will return -1 */
int pipe(int fildes[2]);  /* Uses Windows _pipe() internally */
char *getlogin(void);  /* Uses Windows GetUserName() */
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);

/* Symbolic link operations (Windows Vista+) */
int symlink(const char *target, const char *linkpath);
ssize_t readlink(const char *path, char *buf, size_t bufsize);

/* Temporary file functions */
int mkstemp(char *template_path);

/* popen/pclose - map to Windows _popen/_pclose */
#ifndef popen
#define popen _popen
#endif
#ifndef pclose
#define pclose _pclose
#endif

/* BSD memory functions - map to standard C functions */
#ifndef bcopy
#define bcopy(src, dest, n) memmove((dest), (src), (n))
#endif
#ifndef bzero
#define bzero(s, n) memset((s), 0, (n))
#endif
#ifndef bcmp
#define bcmp(s1, s2, n) memcmp((s1), (s2), (n))
#endif

#ifdef __cplusplus
}
#endif

/* Note: Some functions like fork() and pipe() cannot be directly mapped
 * to Windows equivalents. These will need special handling in the code
 * or alternative implementations using Windows APIs.
 */

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_UNISTD_H */

