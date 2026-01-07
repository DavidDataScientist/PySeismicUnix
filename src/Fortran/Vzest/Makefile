#makefile for vzest

include $(CWPROOT)/src/Makefile.config

# Library linking flags - Windows vs Unix format
ifeq ($(AR),lib)
# Windows: math library is usually linked automatically
# Include Intel Fortran runtime libraries from local lib directory
# libifcoremd.lib - Intel Fortran core runtime (multithreaded DLL)
# ifconsol.lib - Intel Fortran console library
LDLIBS = ifconsol.lib libifcoremd.lib
else
# Unix: use Unix-style flags
LDLIBS =  -lm
endif 

# Please do not leave space at the end
vzestf	        = ./vzestf
vzestsubf       = ./vzestsubf
vzestc	        = ./vzestc

# Object file extension - .obj for Windows, .o for Unix
ifeq ($(AR),lib)
OBJEXT = obj
else
OBJEXT = o
endif

OBJS =  $(vzestf).$(OBJEXT) \
        $(vzestsubf).$(OBJEXT) \
        $(vzestc).$(OBJEXT)

$B/vzest: $(OBJS) 
ifeq ($(AR),lib)
# Windows: link object files with MSVC linker
# /MD is set in FFLAGS and CFLAGS, so object files will use DLL runtime
# Use /NODEFAULTLIB:libcmt to avoid static runtime conflict
# Note: Link order matters - Fortran objects first, then C object
	$(LINK) /nologo /NODEFAULTLIB:libcmt $(vzestf).$(OBJEXT) $(vzestsubf).$(OBJEXT) $(vzestc).$(OBJEXT) $(LDLIBS) /OUT:"$@"
else
# Unix: use Fortran compiler to link
	$(FC) $(OBJS) -o $@ $(LDLIBS)
endif
	@$(MCHMODLINE)
	@echo installing vzest in $B

$(vzestf).$(OBJEXT): $(vzestf).f
ifeq ($(AR),lib)
	$(FC) $(FFLAGS) /Fo$@ $(vzestf).f
else
	$(FC) $(FFLAGS) -c $(vzestf).f -o $@
endif

$(vzestsubf).$(OBJEXT): $(vzestsubf).f
ifeq ($(AR),lib)
	$(FC) $(FFLAGS) /Fo$@ $(vzestsubf).f
else
	$(FC) $(FFLAGS) -c $(vzestsubf).f -o $@
endif

$(vzestc).$(OBJEXT): $(vzestc).c
ifeq ($(AR),lib)
	$(CC) $(CFLAGS) /c /Fo$@ -I. $(vzestc).c
else
	$(CC) $(CFLAGS) -c -I. $(vzestc).c -o $@
endif

INSTALL: $B/vzest
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@echo. > $@ 2>nul || type nul > $@

clean::
	-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul
	-if exist $B/vzest del /F /Q $B/vzest 2>nul
	-if exist ./vzestf del /F /Q ./vzestf 2>nul
	-if exist ./vzestsubf del /F /Q ./vzestsubf 2>nul
	-if exist ./vzestc del /F /Q ./vzestc 2>nul

remake:	
	-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul
	-if exist $B/vzest del /F /Q $B/vzest 2>nul
	-if exist ./vzestf del /F /Q ./vzestf 2>nul
	-if exist ./vzestsubf del /F /Q ./vzestsubf 2>nul
	-if exist ./vzestc del /F /Q ./vzestc 2>nul
	$(MAKE) INSTALL
