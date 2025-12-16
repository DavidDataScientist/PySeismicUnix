# Makefile for ...src/prop/local/Cwell
# Make sure that $(FTARGET) is set in Makefile.config

include $(CWPROOT)/src/Makefile.config

CFLAGS= $(OPTC) -I$I

# Library linking flags - Windows vs Unix format
ifeq ($(AR),lib)
# Windows: use MSVC-style library paths and names
# Include runtime libraries from local lib directory
# libifcoremd.lib - Intel Fortran core runtime (multithreaded DLL)
# ifconsol.lib - Intel Fortran console library
# msvcrt.lib - MSVC runtime library
LFLAGS = $(PRELFLAGS) libpar.lib libcwp.lib msvcrt.lib ifconsol.lib libifcoremd.lib $(POSTLFLAGS)
else
# Unix: use Unix-style flags
LFLAGS= -L$L  -lpar -lcwp -lm
endif

#CPROGS	=	$B/cshotplot
PROGS	=	$B/cwell

# Library name - use .lib extension on Windows, .a on Unix
ifeq ($(AR),lib)
LIB = libcwell.lib
OBJEXT = obj
else
LIB = libcwell.a
OBJEXT = o
endif

# Fortran source files
FSRCS = shoot.f interp.f chkcst.f raydat.f misc.f splines.f bisect.f hwsrc.f graphics.f

# Object files
FOBJS = $(FSRCS:.f=.$(OBJEXT))

# For Windows: compile all sources to objects, then create library
# For Unix: use archive format $(LIB)(file.o)
ifeq ($(AR),lib)
# Windows/MSVC approach: compile to .obj, then create library
%.$(OBJEXT): %.f
	$(FC) $(FFLAGS) /Fo$@ $<

$(LIB): $(FOBJS)
	@-if exist $@ del /F /Q $@ 2>nul
	$(AR) $(ARFLAGS)$@ $(FOBJS)
	$(RANLIB) $@
else
# Unix approach: use archive format
ARCH = $(patsubst %.f,$(LIB)(%.o),$(FSRCS))

$(ARCH): $(LIB)(%.o): %.f
	$(FC) -c $(FFLAGS) $<
	$(AR) $(ARFLAGS) $@ $*.o
	-if exist $*.o del /F /Q $*.o 2>nul

$(LIB): $(ARCH)
	$(RANLIB) $(LIB)
endif

INSTALL:	$(LIB) $(PROGS) $(CPROGS)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@echo. > $@ 2>nul || type nul > $@

# Fortran programs - link with library
ifeq ($(AR),lib)
# Windows: use ifx to compile and link (ifx handles Fortran entry point correctly)
# /MD is set in FFLAGS, so object files will use DLL runtime
# Use /link to pass library paths and libraries
# /NODEFAULTLIB flags prevent wrong runtime libraries from being linked
# Explicitly link modern C runtime (vcruntime140.lib, ucrt.lib) for mainCRTStartup and _fltused
# ifx sets up entry point during compilation phase, before /link
$(PROGS):	$(FTARGET) $(LIB)
	$(FC) /nologo /Zi /MD $(@F).f $(LIB) /link /SUBSYSTEM:CONSOLE /NODEFAULTLIB:libcmt /NODEFAULTLIB:libifcoremt /NODEFAULTLIB:libmmt /NODEFAULTLIB:msvcrt /LIBPATH:"$(L)" vcruntime140.lib ucrt.lib libpar.lib libcwp.lib ifconsol.lib libifcoremd.lib kernel32.lib /OUT:"$@"
	@$(MCHMODLINE)
	@echo $(@F) installed in $B
else
# Unix: use Fortran compiler to link
$(PROGS):	$(FTARGET) $(LIB)
	$(FC) $(FFLAGS) $(@F).f -o $@ $(LIB) $(LFLAGS)
	@$(MCHMODLINE)
	@echo $(@F) installed in $B
endif

#$(CPROGS):     $(CTARGET)
#	-$(CC) $(CFLAGS) $(@F).c $(LFLAGS) -o $@
#	@$(MCHMODLINE)
#	@echo $(@F) installed in $B

remake:
	@-if exist $(LIB) del /F /Q $(LIB) 2>nul
	@-if exist $(PROGS) del /F /Q $(PROGS) 2>nul
	@-if exist $(CPROGS) del /F /Q $(CPROGS) 2>nul
	@-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul
	@$(MAKE) INSTALL

clean::
	-if exist junk* del /F /Q junk* 2>nul
	-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul
	-if exist $(LIB) del /F /Q $(LIB) 2>nul
	-if exist cwell? del /F /Q cwell? 2>nul
	-if exist INSTALL del /F /Q INSTALL 2>nul
