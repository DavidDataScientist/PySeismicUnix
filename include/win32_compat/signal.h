/* Windows compatibility header for signal.h
 * Provides Unix signal handling for Windows/MSVC
 */

#ifndef _WIN32_COMPAT_SIGNAL_H
#define _WIN32_COMPAT_SIGNAL_H

#ifdef _WIN32

#include <signal.h>      /* Windows has signal.h but with different signals */
#include <process.h>     /* For kill() equivalent */
#include <windows.h>     /* For Windows API */

/* Include our types */
#include "win32_compat/sys/types.h"

/* Signal numbers - map Unix signals to Windows equivalents */
#ifndef SIGINT
#define SIGINT    2      /* Interrupt (Ctrl+C) - same on Windows */
#endif

#ifndef SIGILL
#define SIGILL    4      /* Illegal instruction */
#endif

#ifndef SIGFPE
#define SIGFPE    8      /* Floating point exception */
#endif

#ifndef SIGSEGV
#define SIGSEGV   11     /* Segmentation violation */
#endif

#ifndef SIGTERM
#define SIGTERM   15     /* Termination request */
#endif

#ifndef SIGBREAK
#define SIGBREAK  21     /* Break signal (Ctrl+Break on Windows) */
#endif

/* Additional Unix signals that Windows doesn't have directly */
#ifndef SIGHUP
#define SIGHUP    1      /* Hangup - map to SIGTERM on Windows */
#endif

#ifndef SIGQUIT
#define SIGQUIT   3      /* Quit - map to SIGTERM on Windows */
#endif

#ifndef SIGABRT
#define SIGABRT   6      /* Abort - Windows has this */
#endif

#ifndef SIGUSR1
#define SIGUSR1   10     /* User-defined signal 1 - not available on Windows */
#endif

#ifndef SIGUSR2
#define SIGUSR2   12     /* User-defined signal 2 - not available on Windows */
#endif

#ifndef SIGPIPE
#define SIGPIPE   13     /* Broken pipe - not directly available on Windows */
#endif

#ifndef SIGALRM
#define SIGALRM   14     /* Alarm clock - not available on Windows */
#endif

#ifndef SIGCHLD
#define SIGCHLD   17     /* Child process status change - not available on Windows */
#endif

#ifndef SIGCONT
#define SIGCONT   18     /* Continue if stopped - not available on Windows */
#endif

#ifndef SIGSTOP
#define SIGSTOP   19     /* Stop signal - not available on Windows */
#endif

#ifndef SIGTSTP
#define SIGTSTP   20     /* Terminal stop - not available on Windows */
#endif

/* Signal handler type */
typedef void (*sighandler_t)(int);

/* Signal set type (for sigprocmask, etc.) - define before struct sigaction */
typedef struct {
    unsigned long sig[1];  /* Simple implementation */
} sigset_t;

/* sigaction structure */
struct sigaction {
    void (*sa_handler)(int);
    void (*sa_sigaction)(int, void *, void *);
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
};

/* Signal handler constants */
#ifndef SIG_DFL
#define SIG_DFL     ((sighandler_t)0)    /* Default action */
#endif

#ifndef SIG_IGN
#define SIG_IGN     ((sighandler_t)1)    /* Ignore signal */
#endif

#ifndef SIG_ERR
#define SIG_ERR     ((sighandler_t)-1)   /* Error return */
#endif

/* Additional signal numbers */
#ifndef SIGKILL
#define SIGKILL     9      /* Kill signal (cannot be caught) */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* signal() function - Windows has this, we provide a wrapper that maps signals */
/* For Windows-supported signals, we call Windows signal() directly */
/* For unsupported signals, we store handlers internally */
sighandler_t win32_signal(int sig, sighandler_t handler);
sighandler_t win32_compat_signal(int sig, sighandler_t handler);

/* Map signal() to our implementation */
#define signal(sig, handler) win32_compat_signal(sig, handler)

/* kill() function - send signal to process */
int kill(pid_t pid, int sig);

/* raise() function - Windows has this, we provide a wrapper with signal mapping */
int win32_raise(int sig);
int win32_compat_raise(int sig);

/* Map raise() to our implementation */
#define raise(sig) win32_compat_raise(sig)

/* Additional signal functions that might be needed */
int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
int sigprocmask(int how, const sigset_t *set, sigset_t *oset);
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
int sigismember(const sigset_t *set, int signum);
int alarm(unsigned int seconds);
int pause(void);

/* sigprocmask how values */
#define SIG_BLOCK     0
#define SIG_UNBLOCK   1
#define SIG_SETMASK   2

/* sa_flags values */
#define SA_NOCLDSTOP  1
#define SA_NOCLDWAIT  2
#define SA_SIGINFO    4
#define SA_ONSTACK    0x08000000
#define SA_RESTART    0x10000000
#define SA_NODEFER    0x40000000
#define SA_RESETHAND  0x80000000

#ifdef __cplusplus
}
#endif

/* Note: Windows signal handling is more limited than Unix.
 * Some signals (SIGUSR1, SIGUSR2, SIGPIPE, SIGALRM, etc.) are not
 * directly supported and will need special handling or stubs.
 */

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_SIGNAL_H */

