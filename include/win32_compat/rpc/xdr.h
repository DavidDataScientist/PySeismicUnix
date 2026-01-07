/* Windows compatibility header for rpc/xdr.h
 * Provides basic XDR structure and function declarations for Windows/MSVC
 * 
 * Note: This is a minimal implementation. Full XDR support would require
 * a complete XDR library implementation. This header provides the minimum
 * structures and declarations needed to compile the code.
 */

#ifndef _WIN32_COMPAT_RPC_XDR_H
#define _WIN32_COMPAT_RPC_XDR_H

#ifdef _WIN32

#include "win32_compat/rpc/types.h"
#include <stdio.h>

/* XDR operations */
typedef enum {
    XDR_ENCODE = 0,
    XDR_DECODE = 1,
    XDR_FREE = 2
} xdr_op;

/* XDR structure */
typedef struct XDR {
    xdr_op x_op;            /* operation; fast additional param */
    struct xdr_ops {
        bool_t (*x_getlong)(struct XDR *, long *);
        bool_t (*x_putlong)(struct XDR *, const long *);
        bool_t (*x_getbytes)(struct XDR *, char *, unsigned int);
        bool_t (*x_putbytes)(struct XDR *, const char *, unsigned int);
        unsigned int (*x_getpostn)(struct XDR *);
        bool_t (*x_setpostn)(struct XDR *, unsigned int);
        long *(*x_inline)(struct XDR *, unsigned int);
        void (*x_destroy)(struct XDR *);
        bool_t (*x_getint32)(struct XDR *, int32_t *);
        bool_t (*x_putint32)(struct XDR *, const int32_t *);
    } *x_ops;
    char *x_public;         /* users' data */
    char *x_private;        /* pointer to private data */
    char *x_base;           /* private used for position info */
    unsigned int x_handy;   /* extra private word */
} XDR;

/* XDR procedure type - must be defined before use */
typedef bool_t (*xdrproc_t)(XDR *, void *, ...);

/* XDR constants */
#ifndef BYTES_PER_XDR_UNIT
#define BYTES_PER_XDR_UNIT 4
#endif

/* XDR function declarations - minimal stubs */
/* These would need full implementations for actual XDR functionality */
bool_t xdr_long(XDR *xdrs, long *lp);
bool_t xdr_short(XDR *xdrs, short *sp);
bool_t xdr_int(XDR *xdrs, int *ip);
bool_t xdr_u_long(XDR *xdrs, u_long *ulp);
bool_t xdr_u_short(XDR *xdrs, u_short *usp);
bool_t xdr_u_int(XDR *xdrs, u_int *uip);
bool_t xdr_float(XDR *xdrs, float *fp);
bool_t xdr_double(XDR *xdrs, double *dp);
bool_t xdr_char(XDR *xdrs, char *cp);
bool_t xdr_string(XDR *xdrs, char **cpp, u_int maxsize);
bool_t xdr_bytes(XDR *xdrs, char **cpp, u_int *sizep, u_int maxsize);
bool_t xdr_opaque(XDR *xdrs, char *cp, u_int cnt);
bool_t xdr_enum(XDR *xdrs, enum_t *ep);
bool_t xdr_bool(XDR *xdrs, bool_t *bp);
bool_t xdr_array(XDR *xdrs, char **arrp, u_int *sizep, u_int maxsize, u_int elsize, xdrproc_t elproc);
bool_t xdr_vector(XDR *xdrs, char *basep, u_int nelem, u_int elsize, xdrproc_t elproc);
bool_t xdr_pointer(XDR *xdrs, char **objpp, u_int objsize, xdrproc_t xdrobj);
bool_t xdr_wrapstring(XDR *xdrs, char **cpp);
bool_t xdr_void(void);
u_int xdr_getpos(XDR *xdrs);
bool_t xdr_setpos(XDR *xdrs, u_int pos);

/* XDR stream creation functions */
void xdrmem_create(XDR *xdrs, char *addr, u_int size, xdr_op xop);
void xdrstdio_create(XDR *xdrs, FILE *file, xdr_op xop);
void xdr_destroy(XDR *xdrs);

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_RPC_XDR_H */

