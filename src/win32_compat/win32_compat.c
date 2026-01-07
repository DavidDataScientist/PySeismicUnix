/* Windows compatibility function library
 * Implements Unix functions that need actual code (not just macros)
 */

#ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>        /* For time() */
#include <io.h>          /* For _O_* constants and _open, _creat, _pipe, _stat, _fstat */
#include <fcntl.h>       /* For _S_* constants */
#include <process.h>
#include <direct.h>
#include <windows.h>
#include <errno.h>
#include <stdarg.h>

/* Include our compatibility headers */
#include "../include/win32_compat/fcntl.h"
#include "../include/win32_compat/unistd.h"
#include "../include/win32_compat/sys/stat.h"  /* For struct stat definition */
#include "win32_compat.h"

/* Windows _stat is in sys/stat.h - include it after our headers to get _stat */
/* We need to access Windows _stat structure for conversion */
#ifdef _WIN32
/* Forward declare Windows _stat structure members we need */
/* Actually, let's just use the Windows API directly */
#endif

/* fcntl command constants */
#ifndef F_GETFD
#define F_GETFD  1
#define F_SETFD  2
#define F_GETFL  3
#define F_SETFL  4
#endif

/* Ensure _O_* constants are available (they should be from io.h) */
#ifndef _O_RDONLY
#define _O_RDONLY    0x0000
#define _O_WRONLY    0x0001
#define _O_RDWR      0x0002
#define _O_APPEND    0x0008
#define _O_CREAT     0x0100
#define _O_EXCL      0x0400
#define _O_TRUNC     0x0200
#define _O_BINARY    0x8000
#define _O_TEXT      0x4000
#endif

#ifndef _S_IREAD
#define _S_IREAD     0x0100
#define _S_IWRITE    0x0080
#endif

/* File operations that need implementation */
int open(const char *path, int oflag, ...)
{
    int mode = 0;
    
    /* If O_CREAT is specified, get the mode parameter */
    if (oflag & O_CREAT) {
        va_list args;
        va_start(args, oflag);
        mode = va_arg(args, int);
        va_end(args);
    }
    
    /* Convert Unix flags to Windows flags */
    int win_flags = 0;
    
    if (oflag & O_RDONLY) win_flags |= _O_RDONLY;
    if (oflag & O_WRONLY) win_flags |= _O_WRONLY;
    if (oflag & O_RDWR) win_flags |= _O_RDWR;
    if (oflag & O_APPEND) win_flags |= _O_APPEND;
    if (oflag & O_CREAT) win_flags |= _O_CREAT;
    if (oflag & O_EXCL) win_flags |= _O_EXCL;
    if (oflag & O_TRUNC) win_flags |= _O_TRUNC;
    if (oflag & O_BINARY) win_flags |= _O_BINARY;
    if (oflag & O_TEXT) win_flags |= _O_TEXT;
    
    /* Convert mode to Windows _S_IREAD | _S_IWRITE if needed */
    if (mode == 0 && (oflag & O_CREAT)) {
        mode = _S_IREAD | _S_IWRITE;
    }
    
    return _open(path, win_flags, mode);
}

int creat(const char *path, mode_t mode)
{
    int win_mode = _S_IREAD | _S_IWRITE;
    if (mode & S_IRUSR) win_mode |= _S_IREAD;
    if (mode & S_IWUSR) win_mode |= _S_IWRITE;
    
    return _creat(path, win_mode);
}

int fcntl(int fd, int cmd, ...)
{
    /* Windows doesn't have fcntl, but we can provide basic functionality */
    switch (cmd) {
        case F_GETFD:
        case F_SETFD:
        case F_GETFL:
        case F_SETFL:
            /* These are not directly supported on Windows */
            errno = ENOSYS;
            return -1;
        default:
            errno = EINVAL;
            return -1;
    }
}

/* Process operations */
pid_t fork(void)
{
    /* fork() is not available on Windows
     * This is a stub that will need to be replaced with CreateProcess
     * or the code using fork() needs to be refactored
     */
    errno = ENOSYS;
    return -1;
}

int pipe(int fildes[2])
{
    /* Windows doesn't have pipe() in the same way
     * We can use _pipe() which is similar but not identical
     */
    return _pipe(fildes, 4096, _O_BINARY);
}

int dup(int fd)
{
    return _dup(fd);
}

int dup2(int fd1, int fd2)
{
    return _dup2(fd1, fd2);
}

int isatty(int fd)
{
    return _isatty(fd);
}

char *getlogin(void)
{
    /* Windows doesn't have getlogin(), use getenv("USERNAME") */
    static char username[256];
    const char *user = getenv("USERNAME");
    if (user) {
        strncpy(username, user, sizeof(username) - 1);
        username[sizeof(username) - 1] = '\0';
        return username;
    }
    return NULL;
}

int setenv(const char *name, const char *value, int overwrite)
{
    /* Windows doesn't have setenv, use _putenv_s */
    if (!overwrite && getenv(name) != NULL) {
        return 0; /* Variable exists and we're not overwriting */
    }
    
    /* Build "name=value" string */
    size_t len = strlen(name) + strlen(value) + 2;
    char *envstr = (char *)malloc(len);
    if (!envstr) {
        errno = ENOMEM;
        return -1;
    }
    
    snprintf(envstr, len, "%s=%s", name, value);
    int result = _putenv(envstr);
    free(envstr);
    
    return (result == 0) ? 0 : -1;
}

int unsetenv(const char *name)
{
    /* Windows doesn't have unsetenv, use _putenv with "name=" */
    size_t len = strlen(name) + 2;
    char *envstr = (char *)malloc(len);
    if (!envstr) {
        errno = ENOMEM;
        return -1;
    }
    
    snprintf(envstr, len, "%s=", name);
    int result = _putenv(envstr);
    free(envstr);
    
    return (result == 0) ? 0 : -1;
}

/* Symbolic link operations (Windows Vista+) */
int symlink(const char *target, const char *linkpath)
{
    /* Windows has CreateSymbolicLink (Vista+) */
    BOOL result = CreateSymbolicLinkA(linkpath, target, 0);
    if (!result) {
        errno = GetLastError();
        return -1;
    }
    return 0;
}

ssize_t readlink(const char *path, char *buf, size_t bufsize)
{
    /* Windows doesn't have readlink, would need to use GetFinalPathNameByHandle */
    /* For now, return error */
    errno = ENOSYS;
    return -1;
}

/* sys/stat.h functions */
/* Note: We use Windows API directly to avoid header conflicts */
int stat(const char *path, struct stat *buf)
{
    WIN32_FILE_ATTRIBUTE_DATA fad;
    SYSTEMTIME st;
    
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &fad)) {
        errno = ENOENT;
        return -1;
    }
    
    if (buf == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    /* Convert Windows file attributes to Unix stat structure */
    memset(buf, 0, sizeof(struct stat));
    
    /* File type and mode */
    if (fad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        buf->st_mode = S_IFDIR | S_IRWXU | S_IRWXG | S_IRWXO;
    } else {
        buf->st_mode = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }
    
    /* File size */
    buf->st_size = ((__int64)fad.nFileSizeHigh << 32) | fad.nFileSizeLow;
    
    /* Times - convert FILETIME to time_t */
    FileTimeToSystemTime(&fad.ftLastWriteTime, &st);
    /* Simplified time conversion - would need proper implementation */
    buf->st_mtime = time(NULL);  /* Placeholder - needs proper conversion */
    buf->st_atime = buf->st_mtime;
    buf->st_ctime = buf->st_mtime;
    
    /* Other fields */
    buf->st_dev = 0;
    buf->st_ino = 0;
    buf->st_nlink = 1;
    buf->st_uid = 0;
    buf->st_gid = 0;
    buf->st_rdev = 0;
    buf->st_blksize = 4096;
    buf->st_blocks = (buf->st_size + 511) / 512;
    
    return 0;
}

int fstat(int fd, struct stat *buf)
{
    BY_HANDLE_FILE_INFORMATION fileInfo;
    HANDLE hFile;
    
    if (buf == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    /* Get Windows file handle from file descriptor */
    hFile = (HANDLE)_get_osfhandle(fd);
    if (hFile == INVALID_HANDLE_VALUE) {
        errno = EBADF;
        return -1;
    }
    
    if (!GetFileInformationByHandle(hFile, &fileInfo)) {
        errno = EBADF;
        return -1;
    }
    
    /* Convert to Unix stat structure */
    memset(buf, 0, sizeof(struct stat));
    
    /* File type and mode */
    if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        buf->st_mode = S_IFDIR | S_IRWXU | S_IRWXG | S_IRWXO;
    } else {
        buf->st_mode = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }
    
    /* File size */
    buf->st_size = ((__int64)fileInfo.nFileSizeHigh << 32) | fileInfo.nFileSizeLow;
    
    /* Times - simplified */
    buf->st_mtime = time(NULL);  /* Placeholder */
    buf->st_atime = buf->st_mtime;
    buf->st_ctime = buf->st_mtime;
    
    /* Other fields */
    buf->st_dev = 0;
    buf->st_ino = fileInfo.nFileIndexLow;  /* Use file index as inode */
    buf->st_nlink = (nlink_t)fileInfo.nNumberOfLinks;
    buf->st_uid = 0;
    buf->st_gid = 0;
    buf->st_rdev = 0;
    buf->st_blksize = 4096;
    buf->st_blocks = (buf->st_size + 511) / 512;
    
    return 0;
}

int lstat(const char *path, struct stat *buf)
{
    /* On Windows, lstat is the same as stat (no symbolic link distinction in same way) */
    return stat(path, buf);
}

int mkdir(const char *path, mode_t mode)
{
    /* Windows _mkdir doesn't use mode, but we'll call it anyway */
    return _mkdir(path);
}

int mknod(const char *path, mode_t mode, dev_t dev)
{
    /* Windows doesn't support mknod */
    errno = ENOSYS;
    return -1;
}

int chmod(const char *path, mode_t mode)
{
    /* Windows _chmod uses different mode values */
    /* Map Unix permissions to Windows:
     * S_IRUSR (read) -> _S_IREAD
     * S_IWUSR (write) -> _S_IWRITE
     * S_IXUSR (execute) -> not directly supported, but we can set it
     * Group and other permissions are not supported on Windows
     */
    int win_mode = 0;
    
    /* User permissions */
    if (mode & S_IRUSR) {
        win_mode |= _S_IREAD;
    }
    if (mode & S_IWUSR) {
        win_mode |= _S_IWRITE;
    }
    
    /* On Windows, if read is set, we typically also allow execute for files */
    /* This is a simplification - Windows doesn't have execute bits the same way */
    if (mode & S_IXUSR || mode & S_IRUSR) {
        /* Windows doesn't have separate execute permission */
        /* Files are executable if they have .exe, .bat, etc. extensions */
    }
    
    /* If no permissions specified, default to read+write */
    if (win_mode == 0) {
        win_mode = _S_IREAD | _S_IWRITE;
    }
    
    return _chmod(path, win_mode);
}

/* umask is provided by Windows, but with different semantics */
/* We'll leave it as-is for now - code can use Windows umask directly */

/* signal.h functions */
#include "../include/win32_compat/signal.h"

/* Signal handler storage for signals not directly supported by Windows */
static sighandler_t signal_handlers[32] = {0};

/* Get Windows signal() function pointer */
/* Windows signal() signature: void (*signal(int sig, void (*func)(int)))(int) */
typedef void (*(*win_signal_func_t)(int, void (*)(int)))(int);
static win_signal_func_t get_win_signal(void)
{
    static win_signal_func_t win_signal_ptr = NULL;
    if (win_signal_ptr == NULL) {
        /* Use LoadLibrary/GetProcAddress to get Windows signal() */
        HMODULE hModule = GetModuleHandleA("msvcrt.dll");
        if (hModule != NULL) {
            win_signal_ptr = (win_signal_func_t)GetProcAddress(hModule, "signal");
        }
    }
    return win_signal_ptr;
}

/* Use a different name to avoid linker conflicts with Windows signal() */
sighandler_t win32_compat_signal(int sig, sighandler_t handler)
{
    int win_sig;
    sighandler_t old_handler;
    win_signal_func_t win_signal_func;
    
    /* Map Unix signals to Windows signals */
    switch (sig) {
        case SIGINT:
            win_sig = SIGINT;
            break;
        case SIGTERM:
        case SIGHUP:
        case SIGQUIT:
            win_sig = SIGINT;  /* Map to SIGINT */
            break;
        case SIGFPE:
            win_sig = SIGFPE;
            break;
        case SIGSEGV:
            win_sig = SIGSEGV;
            break;
        case SIGABRT:
            win_sig = SIGABRT;
            break;
        case SIGBREAK:
            win_sig = SIGBREAK;
            break;
        default:
            /* For unsupported signals, just store handler */
            if (sig >= 0 && sig < 32) {
                old_handler = signal_handlers[sig];
                signal_handlers[sig] = handler;
                return old_handler;
            }
            return SIG_ERR;
    }
    
    /* Store handler */
    if (sig >= 0 && sig < 32) {
        old_handler = signal_handlers[sig];
        signal_handlers[sig] = handler;
    } else {
        old_handler = SIG_ERR;
    }
    
    /* Call Windows signal() via function pointer */
    win_signal_func = get_win_signal();
    if (win_signal_func != NULL) {
        void (*win_result)(int);
        win_result = win_signal_func(win_sig, (void (*)(int))handler);
        return (sighandler_t)win_result;
    }
    
    return old_handler;
}

sighandler_t win32_signal(int sig, sighandler_t handler)
{
    /* Alias for win32_compat_signal() */
    return win32_compat_signal(sig, handler);
}

int kill(pid_t pid, int sig)
{
    /* Windows doesn't have kill() in the same way */
    /* We can use TerminateProcess for SIGTERM/SIGKILL */
    
    if (pid <= 0) {
        errno = EINVAL;
        return -1;
    }
    
    /* For current process */
    if (pid == getpid()) {
        return raise(sig);
    }
    
    /* For other processes, we'd need to use OpenProcess and TerminateProcess */
    /* This is a simplified implementation */
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, (DWORD)pid);
    if (hProcess == NULL) {
        errno = ESRCH;  /* No such process */
        return -1;
    }
    
    BOOL result = FALSE;
    if (sig == SIGTERM || sig == SIGKILL) {
        result = TerminateProcess(hProcess, 1);
    } else {
        /* Other signals not supported for remote processes on Windows */
        CloseHandle(hProcess);
        errno = EINVAL;
        return -1;
    }
    
    CloseHandle(hProcess);
    
    if (!result) {
        errno = EPERM;  /* Permission denied */
        return -1;
    }
    
    return 0;
}

/* Get Windows raise() function pointer */
typedef int (*win_raise_func_t)(int);
static win_raise_func_t get_win_raise(void)
{
    static win_raise_func_t win_raise_ptr = NULL;
    if (win_raise_ptr == NULL) {
        HMODULE hModule = GetModuleHandleA("msvcrt.dll");
        if (hModule != NULL) {
            win_raise_ptr = (win_raise_func_t)GetProcAddress(hModule, "raise");
        }
    }
    return win_raise_ptr;
}

/* Use a different name to avoid linker conflicts with Windows raise() */
int win32_compat_raise(int sig)
{
    int win_sig;
    win_raise_func_t win_raise_func;
    
    switch (sig) {
        case SIGINT:
        case SIGTERM:
        case SIGHUP:
        case SIGQUIT:
            win_sig = SIGINT;
            break;
        case SIGFPE:
            win_sig = SIGFPE;
            break;
        case SIGSEGV:
            win_sig = SIGSEGV;
            break;
        case SIGABRT:
            win_sig = SIGABRT;
            break;
        case SIGBREAK:
            win_sig = SIGBREAK;
            break;
        default:
            /* For unsupported signals, call handler directly if set */
            if (sig >= 0 && sig < 32 && signal_handlers[sig] != NULL && signal_handlers[sig] != SIG_DFL && signal_handlers[sig] != SIG_IGN) {
                signal_handlers[sig](sig);
                return 0;
            }
            errno = EINVAL;
            return -1;
    }
    
    /* Call Windows raise() via function pointer */
    win_raise_func = get_win_raise();
    if (win_raise_func != NULL) {
        return win_raise_func(win_sig);
    }
    
    errno = ENOSYS;
    return -1;
}

int win32_raise(int sig)
{
    /* Alias for win32_compat_raise() */
    return win32_compat_raise(sig);
}

/* Stub implementations for advanced signal functions */
int sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
    /* Windows doesn't support sigaction, use signal() instead */
    if (act != NULL) {
        sighandler_t old = signal(sig, act->sa_handler);
        if (oact != NULL && old != SIG_ERR) {
            oact->sa_handler = old;
            sigemptyset(&oact->sa_mask);
            oact->sa_flags = 0;
        }
        return (old == SIG_ERR) ? -1 : 0;
    }
    return 0;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
    /* Windows doesn't support signal masks */
    /* Stub implementation */
    if (oset != NULL) {
        sigemptyset(oset);
    }
    return 0;
}

int sigemptyset(sigset_t *set)
{
    if (set == NULL) {
        errno = EINVAL;
        return -1;
    }
    set->sig[0] = 0;
    return 0;
}

int sigfillset(sigset_t *set)
{
    if (set == NULL) {
        errno = EINVAL;
        return -1;
    }
    set->sig[0] = ~0UL;
    return 0;
}

int sigaddset(sigset_t *set, int signum)
{
    if (set == NULL || signum < 0 || signum >= 32) {
        errno = EINVAL;
        return -1;
    }
    set->sig[0] |= (1UL << signum);
    return 0;
}

int sigdelset(sigset_t *set, int signum)
{
    if (set == NULL || signum < 0 || signum >= 32) {
        errno = EINVAL;
        return -1;
    }
    set->sig[0] &= ~(1UL << signum);
    return 0;
}

int sigismember(const sigset_t *set, int signum)
{
    if (set == NULL || signum < 0 || signum >= 32) {
        errno = EINVAL;
        return -1;
    }
    return (set->sig[0] & (1UL << signum)) ? 1 : 0;
}

int alarm(unsigned int seconds)
{
    /* Windows doesn't have alarm(), would need to use SetTimer or similar */
    /* Stub implementation */
    (void)seconds;  /* Unused */
    errno = ENOSYS;
    return 0;
}

int pause(void)
{
    /* Windows doesn't have pause(), use Sleep(INFINITE) */
    Sleep(INFINITE);
    return -1;  /* Should not return, but if it does... */
}

/* Temporary file functions */
int mkstemp(char *template)
{
    /* Windows equivalent of mkstemp */
    /* Creates a unique temporary file from template */
    /* Template should end with "XXXXXX" which will be replaced */
    
    if (template == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    /* Find the XXXXXX pattern */
    char *pattern = strstr(template, "XXXXXX");
    if (pattern == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    /* Generate a unique filename */
    char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int attempts = 0;
    int max_attempts = 1000;
    
    while (attempts < max_attempts) {
        /* Fill XXXXXX with random characters */
        for (int i = 0; i < 6; i++) {
            pattern[i] = chars[rand() % 62];
        }
        
        /* Try to create the file exclusively */
        int fd = _open(template, _O_CREAT | _O_EXCL | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
        if (fd >= 0) {
            return fd;
        }
        
        /* If file exists, try again */
        if (errno == EEXIST) {
            attempts++;
            continue;
        }
        
        /* Other error */
        return -1;
    }
    
    /* Too many attempts */
    errno = EEXIST;
    return -1;
}

FILE *fdopen(int fd, const char *mode)
{
    /* Windows equivalent of fdopen */
    /* Opens a file descriptor as a FILE stream */
    
    if (fd < 0 || mode == NULL) {
        errno = EINVAL;
        return NULL;
    }
    
    /* Map mode string to Windows flags */
    int binary = 0;
    char win_mode[4] = {0};
    int pos = 0;
    
    if (strchr(mode, 'r') != NULL) {
        win_mode[pos++] = 'r';
    }
    if (strchr(mode, 'w') != NULL) {
        win_mode[pos++] = 'w';
    }
    if (strchr(mode, 'a') != NULL) {
        win_mode[pos++] = 'a';
    }
    if (strchr(mode, '+') != NULL) {
        win_mode[pos++] = '+';
    }
    if (strchr(mode, 'b') != NULL) {
        binary = 1;
        win_mode[pos++] = 'b';
    }
    win_mode[pos] = '\0';
    
    /* Use _fdopen which is the Windows equivalent */
    return _fdopen(fd, win_mode);
}

/* popen/pclose - wrappers for Windows _popen/_pclose
 * These are needed because libpar was compiled with references to popen/pclose
 * Use #undef to avoid macro expansion causing recursion
 */
#undef popen
#undef pclose

FILE *popen(const char *command, const char *mode)
{
    return _popen(command, mode);
}

int pclose(FILE *stream)
{
    return _pclose(stream);
}

#endif /* _WIN32 */

