/* Windows compatibility header for sys/stat.h
 * Provides file status functions and structures for Windows/MSVC
 */

#ifndef _WIN32_COMPAT_SYS_STAT_H
#define _WIN32_COMPAT_SYS_STAT_H

#ifdef _WIN32

/* Include our types first to avoid conflicts */
#include "win32_compat/sys/types.h"

#include <io.h>          /* For _stat, _fstat */
/* Don't include Windows sys/stat.h here - it conflicts with our struct stat */
/* We'll use _stat from io.h and map it to our stat structure */

/* File type macros */
#ifndef S_IFMT
#define S_IFMT   0170000  /* File type mask */
#endif

#ifndef S_IFDIR
#define S_IFDIR  0040000  /* Directory */
#endif

#ifndef S_IFCHR
#define S_IFCHR  0020000  /* Character device */
#endif

#ifndef S_IFBLK
#define S_IFBLK  0060000  /* Block device */
#endif

#ifndef S_IFREG
#define S_IFREG  0100000  /* Regular file */
#endif

#ifndef S_IFIFO
#define S_IFIFO  0010000  /* FIFO/pipe */
#endif

#ifndef S_IFLNK
#define S_IFLNK  0120000  /* Symbolic link */
#endif

#ifndef S_IFSOCK
#define S_IFSOCK 0140000  /* Socket */
#endif

/* File permission bits (already defined in fcntl.h, but include here for completeness) */
#ifndef S_IRUSR
#define S_IRUSR   0400    /* Read permission, owner */
#define S_IWUSR   0200    /* Write permission, owner */
#define S_IXUSR   0100    /* Execute permission, owner */
#define S_IRGRP   0040    /* Read permission, group */
#define S_IWGRP   0020    /* Write permission, group */
#define S_IXGRP   0010    /* Execute permission, group */
#define S_IROTH   0004    /* Read permission, other */
#define S_IWOTH   0002    /* Write permission, other */
#define S_IXOTH   0001    /* Execute permission, other */

#define S_IRWXU   (S_IRUSR | S_IWUSR | S_IXUSR)  /* Read, write, execute by owner */
#define S_IRWXG   (S_IRGRP | S_IWGRP | S_IXGRP)  /* Read, write, execute by group */
#define S_IRWXO   (S_IROTH | S_IWOTH | S_IXOTH)  /* Read, write, execute by other */
#endif

/* Additional permission bits */
#ifndef S_ISUID
#define S_ISUID   04000   /* Set user ID on execution */
#define S_ISGID   02000   /* Set group ID on execution */
#define S_ISVTX   01000   /* Sticky bit */
#endif

/* Convenience macros for file type checking */
#ifndef S_ISDIR
#define S_ISDIR(m)    (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)    (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)    (((m) & S_IFMT) == S_IFBLK)
#define S_ISREG(m)    (((m) & S_IFMT) == S_IFREG)
#define S_ISFIFO(m)   (((m) & S_IFMT) == S_IFIFO)
#define S_ISLNK(m)    (((m) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m)   (((m) & S_IFMT) == S_IFSOCK)
#endif

/* Type definitions for stat structure members - define before struct */
#ifndef _NLINK_T_DEFINED
#define _NLINK_T_DEFINED
typedef unsigned short nlink_t;
#endif

#ifndef _BLKSIZE_T_DEFINED
#define _BLKSIZE_T_DEFINED
typedef long blksize_t;
#endif

#ifndef _BLKCNT_T_DEFINED
#define _BLKCNT_T_DEFINED
typedef __int64 blkcnt_t;
#endif

/* Unix-style stat structure - use different name to avoid conflict with Windows _stat */
#ifndef _WIN32_STAT_DEFINED
#define _WIN32_STAT_DEFINED
struct stat {
    dev_t     st_dev;      /* Device ID of device containing file */
    ino_t     st_ino;      /* Inode number */
    mode_t    st_mode;     /* File type and mode */
    nlink_t   st_nlink;    /* Number of hard links */
    uid_t     st_uid;      /* User ID of owner */
    gid_t     st_gid;      /* Group ID of owner */
    dev_t     st_rdev;     /* Device ID (if special file) */
    off_t     st_size;     /* Total size, in bytes */
    time_t    st_atime;    /* Time of last access */
    time_t    st_mtime;    /* Time of last modification */
    time_t    st_ctime;    /* Time of last status change */
    blksize_t st_blksize;  /* Block size for filesystem I/O */
    blkcnt_t  st_blocks;   /* Number of 512-byte blocks allocated */
};
#endif

/* Function declarations */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * stat/fstat/lstat - we provide our own implementation that populates
 * more fields than Windows' _stat
 */
int stat(const char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);

/* mknod - does not exist in Windows */
int mknod(const char *path, mode_t mode, dev_t dev);

/*
 * mkdir, chmod - DO NOT redeclare, Windows has these in <direct.h> and <io.h>
 * The Windows versions have slightly different signatures but work for most cases.
 * Use _mkdir() and _chmod() directly via the macros defined above.
 */

#ifdef __cplusplus
}
#endif

/* Implementation note:
 * These functions will need to be implemented in win32_compat.c
 * They will convert between Windows _stat structure and Unix stat structure
 */

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_SYS_STAT_H */

