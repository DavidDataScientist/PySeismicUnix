/* Windows compatibility header for rpc/types.h
 * Provides basic RPC/XDR type definitions for Windows/MSVC
 * 
 * Note: This is a minimal implementation. Full XDR support would require
 * a complete XDR library implementation, which is beyond the scope of
 * this compatibility layer. This header provides the minimum types needed
 * to compile the code.
 */

#ifndef _WIN32_COMPAT_RPC_TYPES_H
#define _WIN32_COMPAT_RPC_TYPES_H

#ifdef _WIN32

#include <stddef.h>
#include <stdint.h>

/* Basic types used by XDR */
#ifndef bool_t
typedef int bool_t;
#endif

#ifndef enum_t
typedef int enum_t;
#endif

#ifndef u_long
typedef unsigned long u_long;
#endif

#ifndef u_short
typedef unsigned short u_short;
#endif

#ifndef u_char
typedef unsigned char u_char;
#endif

#ifndef u_int
typedef unsigned int u_int;
#endif

/* XDR constants */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_RPC_TYPES_H */

