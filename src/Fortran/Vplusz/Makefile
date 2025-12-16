#makefile for vplusz

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
LDLIBS = -lm
endif 

S = .

vpluszf = $S/vpluszf
vpluszc = $S/vpluszc

# Object file extension - .obj for Windows, .o for Unix
ifeq ($(AR),lib)
OBJEXT = obj
else
OBJEXT = o
endif

OBJS = $(vpluszf).$(OBJEXT) \
       $(vpluszc).$(OBJEXT)

$B/vplusz: $(OBJS) 
ifeq ($(AR),lib)
# Windows: link object files with MSVC linker
# /MD is set in FFLAGS and CFLAGS, so object files will use DLL runtime
# Use /NODEFAULTLIB:libcmt to avoid static runtime conflict
	$(LINK) /nologo /NODEFAULTLIB:libcmt $(OBJS) $(LDLIBS) /OUT:"$@"
else
# Unix: use Fortran compiler to link
	$(FC) $(FFLAGS) $(OBJS) -o $@ $(LDLIBS)
endif
	@$(MCHMODLINE)
	@echo installing vplusz in $B

$(vpluszf).$(OBJEXT): $(vpluszf).f
ifeq ($(AR),lib)
	$(FC) $(FFLAGS) /Fo$@ $(vpluszf).f
else
	$(FC) $(FFLAGS) -c $(vpluszf).f -o $@
endif

$(vpluszc).$(OBJEXT): $(vpluszc).c
ifeq ($(AR),lib)
	$(CC) $(CFLAGS) /c /Fo$@ $(vpluszc).c
else
	$(CC) $(CFLAGS) -c $(vpluszc).c -o $@
endif

INSTALL: $B/vplusz
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@echo. > $@ 2>nul || type nul > $@

clean::
	-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul
	-if exist $B/vplusz del /F /Q $B/vplusz 2>nul

remake:
	-if exist *.$(OBJEXT) del /F /Q *.$(OBJEXT) 2>nul
	-if exist $B/vplusz del /F /Q $B/vplusz 2>nul
	$(MAKE) INSTALL
