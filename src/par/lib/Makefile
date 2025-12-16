# Makefile for ...par/lib

include $(CWPROOT)/src/Makefile.config

D = $I/par.h

LFLAGS= -L$L -lcwp

# Library name - use .lib extension on Windows, .a on Unix
# Check if AR is "lib" (Windows) or "ar" (Unix)
ifeq ($(AR),lib)
LIB = $L/libpar.lib
OBJEXT = obj
else
LIB = $L/libpar.a
OBJEXT = o
endif

# Source files
SRCS = atopkge.c docpkge.c ealloc.c errpkge.c filestat.c fractal.c \
	getpars.c lincoeff.c minfunc.c modeling.c refaniso.c rke.c \
	smooth.c syscalls.c subcalls.c taup.c VND.c upweik.c wtlib.c

# Object files
OBJS = $(SRCS:.c=.$(OBJEXT))

# For Windows: compile all sources to objects, then create library
# For Unix: use archive format $(LIB)(file.o)
ifeq ($(AR),lib)
# Windows/MSVC approach: compile to .obj, then create library
# Pattern rule: compile each .c file to .obj file
%.$(OBJEXT): %.c $D
	$(CC) $(CFLAGS) /c /Fo$@ $<

$(LIB): $(OBJS)
	@-if exist $@ del /F /Q $@ 2>nul
	$(AR) $(ARFLAGS)$@ $(OBJS)
	$(RANLIB) $@
else
# Unix approach: use archive format
ARCH = $(patsubst %.c,$(LIB)(%.o),$(SRCS))

$(ARCH): $(LIB)(%.o): %.c $D
	$(CC) -c $(CFLAGS) $<
	$(AR) $(ARFLAGS) $@ $*.o
	-if exist $*.o del /F /Q $*.o 2>nul

$(LIB): $(ARCH)
	$(RANLIB) $(LIB)
endif

INSTALL:	$(LIB) $L
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@echo. > $@ 2>nul || type nul > $@

remake	:
	@-if exist $(LIB) del /F /Q $(LIB) 2>nul
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul
	@$(MAKE) INSTALL

list	:
	$(AR) /LIST $(LIB)

clean::
	-if exist junk* del /F /Q junk* 2>nul
	-if exist core del /F /Q core 2>nul
	-if exist a.out del /F /Q a.out 2>nul
	-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul

.PRECIOUS:	 $(LIB)
