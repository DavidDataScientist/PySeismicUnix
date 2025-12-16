# Makefile for ...su/lib

include $(CWPROOT)/src/Makefile.config

D = $I/su.h

LFLAGS= -L$L -lpar -lcwp $(XDRLFLAGS) -lm
TFLAGS = -g -I$I

# Library name - use .lib extension on Windows, .a on Unix
# Check if AR is "lib" (Windows) or "ar" (Unix)
ifeq ($(AR),lib)
LIB = $L/libsu.lib
OBJEXT = obj
else
LIB = $L/libsu.a
OBJEXT = o
endif

# Source files
SRCS = elco_scalar.c fgettr.c fgethdr.c fputtr.c fgetgthr.c fputgthr.c \
	headcase.c readkfile.c qdefine.c bilinear.c linterpd.c gridread.c \
	gridxy.c hdrpkge.c valpkge.c tabplot.c getSPSfield.c xdrbhdrsub.c \
	xdrhdrsub.c

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
	@echo. > $@ 2>nul || type nul > $@


remake	:
	@-if exist $(LIB) del /F /Q $(LIB) 2>nul
	@-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul
	@$(MAKE) INSTALL

list	:
	$(AR) /LIST $(LIB)


# Testing routines
tests: tgettr tputtr tarray

tgettr:	fgettr.c
	$(CC) $(TFLAGS) -DTEST -DGETTR fgettr.c $(LIB) $(LFLAGS)  -o tgettr
	@echo tgettr ready--running benchmark
	suplane offset=-100 | sugethw offset
	@echo convert offset to absolute value
	suplane offset=-100 | tgettr | sugethw offset

tputtr:	fputtr.c
	$(CC) $(TFLAGS) -DTEST -DPUTTR fputtr.c $(LIB) $(LFLAGS) -o tputtr
	@echo tputtr ready--running benchmark
	suplane offset=-100 | sugethw offset
	@echo convert offset to absolute value
	suplane offset=-100 | tputtr | sugethw offset

tarray:	arraypkge.c
	$(CC) $(TFLAGS) -DTEST arraypkge.c $(LIB) $(LFLAGS) -o tarray
	@echo arraypkge ready--running benchmark
	-tarray

clean::
	-if exist ato del /F /Q ato 2>nul
	-if exist err del /F /Q err 2>nul
	-if exist io del /F /Q io 2>nul
	-if exist tarray del /F /Q tarray 2>nul
	-if exist tgetname del /F /Q tgetname 2>nul
	-if exist tgetpar del /F /Q tgetpar 2>nul
	-if exist tgettr del /F /Q tgettr 2>nul
	-if exist tputtr del /F /Q tputtr 2>nul
	-if exist tstatfil del /F /Q tstatfil 2>nul
	-if exist junk* del /F /Q junk* 2>nul
	-if exist core del /F /Q core 2>nul
	-if exist a.out del /F /Q a.out 2>nul
	-if exist xdrbhdrsub.* del /F /Q xdrbhdrsub.* 2>nul
	-if exist xdrhdrsub.* del /F /Q xdrhdrsub.* 2>nul
	-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul

.PRECIOUS: $(LIB)
