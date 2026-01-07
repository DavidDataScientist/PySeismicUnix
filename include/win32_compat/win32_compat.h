/* Windows compatibility library header
 * Declares functions provided by win32_compat.c
 */

#ifndef _WIN32_COMPAT_LIB_H
#define _WIN32_COMPAT_LIB_H

#ifdef _WIN32

#include "../include/win32_compat/sys/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Function declarations for compatibility functions */
int open(const char *path, int oflag, ...);
int creat(const char *path, mode_t mode);
int fcntl(int fd, int cmd, ...);
pid_t fork(void);
int pipe(int fildes[2]);
int dup(int fd);
int dup2(int fd1, int fd2);
int isatty(int fd);
char *getlogin(void);
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);
int symlink(const char *target, const char *linkpath);
ssize_t readlink(const char *path, char *buf, size_t bufsize);

/* sys/stat.h functions */
int stat(const char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);
int mkdir(const char *path, mode_t mode);
int mknod(const char *path, mode_t mode, dev_t dev);
int chmod(const char *path, mode_t mode);
/* umask is provided by Windows headers */

/* Path compatibility functions */
char *win32_path_from_unix(const char *unix_path);
char *win32_path_from_unix_inplace(char *path, size_t bufsize);
char *win32_normalize_path_separators(char *path);
char *win32_get_temp_dir(void);
char *win32_get_home_dir(void);
int win32_is_absolute_path(const char *path);
char *win32_path_join(const char *base, ...);
char *win32_map_unix_path(const char *unix_path);

/* Temporary file functions */
int mkstemp(char *template);
FILE *fdopen(int fd, const char *mode);

/* signal.h functions are declared in include/win32_compat/signal.h */
/* Include that header to get the declarations */

#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_LIB_H */

