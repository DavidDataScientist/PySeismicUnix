#ifndef _STDIO_S_H
#define _STDIO_S_H	1


#include	"FEATURE/sfio"

#if _typ___FILE /* Redhat7.3 requires __FILE in wchar.h */
#if CWP_RED_HAT_7_3
typedef struct _sfio_s	*__FILE;
#endif
#endif

#include	"sfhdr.h"
#include	"stdio.h"

#endif /*_STDIO_S_H*/

