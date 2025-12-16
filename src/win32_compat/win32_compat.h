/* Windows compatibility library header
 * Declares functions provided by win32_compat.c
 * 
 * NOTE: Do NOT redeclare functions that Windows SDK provides (open, creat, 
 * dup, dup2, isatty, mkdir, chmod, fdopen) - they cause "inconsistent dll 
 * linkage" warnings. Only declare functions we actually implement.
 */

#ifndef _WIN32_COMPAT_LIB_H
#define _WIN32_COMPAT_LIB_H

#ifdef _WIN32

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Functions that don't exist in Windows - we provide implementations */
int fcntl(int fd, int cmd, ...);
pid_t fork(void);
int pipe(int fildes[2]);
char *getlogin(void);
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);
int symlink(const char *target, const char *linkpath);
ssize_t readlink(const char *path, char *buf, size_t bufsize);
int mkstemp(char *template_path);
int mknod(const char *path, mode_t mode, dev_t dev);

/* popen/pclose wrappers (libpar references these without underscore) */
FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);

/* Custom stat functions that provide more Unix-like behavior */
struct stat;
int stat(const char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);

/* Path compatibility functions */
char *win32_path_from_unix(const char *unix_path);
char *win32_path_from_unix_inplace(char *path, size_t bufsize);
char *win32_normalize_path_separators(char *path);
char *win32_get_temp_dir(void);
char *win32_get_home_dir(void);
int win32_is_absolute_path(const char *path);
char *win32_path_join(const char *base, ...);
char *win32_map_unix_path(const char *unix_path);

#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_LIB_H */

