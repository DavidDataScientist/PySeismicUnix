/* Copyright (c) Colorado School of Mines, 2011.*/
/* All rights reserved.                       */

/*********************** self documentation **********************/
/*************************************************************************** 
DOCPKGE - Function to implement the CWP self-documentation facility

requestdoc	give selfdoc on user request (i.e. when name of main is typed)
pagedoc		print self documentation string

**************************************************************************** 
Function Prototypes:
void requestdoc(flag);
void pagedoc();

**************************************************************************** 
requestoc:
Input:
flag		integer specifying i.o. cases

pagedoc():
Returns:	the self-documentation, an array of strings

**************************************************************************** 
Notes:
requestdoc:
In the usual case, stdin is used to pass in data.  However,
some programs (eg. synthetic data generators) don't use stdin
to pass in data and some programs require two or more arguments
besides the command itself (eg. sudiff) and don't use stdin.
In this last case, we give selfdoc whenever too few arguments
are given, since these usages violate the usual SU syntax.
In all cases, selfdoc can be requested by giving only the
program name.

The flag argument distinguishes these cases:
            flag = 0; fully defaulted, no stdin
            flag = 1; usual case
            flag = n > 1; no stdin and n extra args required

pagedoc:
Intended to be called by requesdoc(), but conceivably could be
used directly as in:
      if (xargc != 3) selfdoc();

Based on earlier versions by:
SEP: Einar Kjartansson, Stew Levin CWP: Jack Cohen, Shuki Ronen
HRC: Lyle

**************************************************************************** 
Author: Jack K. Cohen, Center for Wave Phenomena
****************************************************************************/
/**************** end self doc ********************************/


#include "par.h"
#include <string.h>
 
/*  definitions of global variables */
int xargc; char **xargv;

/* Forward declaration */
void pagedoc_help(void);

void requestdoc(int flag)
/*************************************************************************** 
print selfdocumentation as directed by the user-specified flag
**************************************************************************** 
Notes:
In the usual case, stdin is used to pass in data.  However,
some programs (eg. synthetic data generators) don't use stdin
to pass in data and some programs require two or more arguments
besides the command itself (eg. sudiff) and don't use stdin.
In this last case, we give selfdoc whenever too few arguments
are given, since these usages violate the usual SU syntax.
In all cases, selfdoc can be requested by giving only the
program name, or by using --help or -h flags.

The flag argument distinguishes these cases:
            flag = 0; fully defaulted, no stdin
            flag = 1; usual case
            flag = n > 1; no stdin and n extra args required

pagedoc:
Intended to be called by pagedoc(), but conceivably could be
used directly as in:
      if (xargc != 3) selfdoc();

**************************************************************************** 
Authors: Jack Cohen, Center for Wave Phenomena, 1993, based on on earlier
versions by:
SEP: Einar Kjartansson, Stew Levin CWP: Jack Cohen, Shuki Ronen
HRC: Lyle
Windows port: Added --help and -h support
****************************************************************************/
{
        /* Check for --help or -h flags (Windows port enhancement) */
        /* This check must happen BEFORE any stdin operations */
        if (xargc >= 2 && xargv != NULL && xargv[1] != NULL) {
                if (strcmp(xargv[1], "--help") == 0 || strcmp(xargv[1], "-h") == 0) {
                        pagedoc_help();  /* Direct output version for --help */
                        /* pagedoc_help() exits with 1 if no help, 0 if help found */
                        /* If we get here, help was found, so exit with 0 */
                        fflush(stdout);
                        fflush(stderr);
                        exit(0);  /* Exit immediately after printing docs */
                }
        }
        
        switch(flag) {
        case 1:
                if (xargc == 1 && isatty(STDIN)) pagedoc();
        break;
        case 0:
                if (xargc == 1 && isatty(STDIN) && isatty(STDOUT)) pagedoc();
        break;
        default:
                if (xargc <= flag) pagedoc();
        break;
        }
        return;
}


void pagedoc(void)
{
        extern char *sdoc[];
		char **p = sdoc;
        FILE *fp;
		char *pager;
		char cmd[32];

		if ((pager=getenv("PAGER")) != (char *)NULL)
			sprintf(cmd,"%s 1>&2", pager);
		else 
			sprintf(cmd,"more 1>&2");


        fflush(stdout);
       /*  fp = popen("more -22 1>&2", "w"); */
       /*  fp = popen("more  1>&2", "w"); */
        fp = (FILE *) popen(cmd, "w");
	while(*p) (void)fprintf(fp, "%s\n", *p++);
        pclose(fp);

        exit(EXIT_FAILURE);
}

/* Windows port: Direct help output for --help flag (bypasses pager) */
void pagedoc_help(void)
{
        extern char *sdoc[];
		char **p;
		int has_help = 0;

        fflush(stdout);
        fflush(stderr);
	
	/* Check if help documentation is available */
	if (sdoc != NULL && sdoc[0] != NULL) {
		/* Write to stdout for better compatibility with scripts */
		p = sdoc;
		while(*p != NULL) {
			(void)fprintf(stdout, "%s\n", *p++);
			has_help = 1;
		}
	}
	
	/* If no help available, print error and exit with non-zero code */
	if (!has_help) {
		(void)fprintf(stderr, "No help available for this program.\n");
		fflush(stderr);
		exit(1);  /* Exit with error code - no man page should be created */
	}
	
        fflush(stdout);
        fflush(stderr);
        /* Note: exit(0) is called by requestdoc() after this function returns if help was found */
}
