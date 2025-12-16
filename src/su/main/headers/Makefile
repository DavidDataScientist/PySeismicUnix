# Makefile for ...su/main/headers

# Use backslashes for Windows paths
ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

# Executable extension and linker flags
ifeq ($(AR),lib)
EXEEXT = .exe
D = $(L)\libsu.lib $(L)\libpar.lib $(L)\libcwp.lib $(L)\libwin32compat.lib
LFLAGS = $(PRELFLAGS) libsu.lib libpar.lib libcwp.lib libwin32compat.lib $(POSTLFLAGS)
else
EXEEXT =
D = $L/libcwp.a $L/libpar.a $L/libsu.a
LFLAGS = $(PRELFLAGS) -L$L -lsu -lpar -lcwp -lm $(POSTLFLAGS)
endif

# Program list - built locally first on Windows
LOCAL_PROGS =		\
	bhedtopar$(EXEEXT)	\
	su3dchart$(EXEEXT)	\
	suazimuth$(EXEEXT)	\
	suabshw$(EXEEXT)	\
	suahw$(EXEEXT)		\
	suaddhead$(EXEEXT)	\
	subincsv$(EXEEXT)	\
	subinqcsv$(EXEEXT)	\
	subinxxcsv$(EXEEXT)	\
	subinbigcsv$(EXEEXT)	\
	sucdpbin$(EXEEXT)	\
	suchart$(EXEEXT)	\
	suchw$(EXEEXT)		\
	sucliphead$(EXEEXT)	\
	sucountkey$(EXEEXT)	\
	sudumptrace$(EXEEXT)	\
	suedit$(EXEEXT)		\
	suflex2d$(EXEEXT)	\
	sugethw$(EXEEXT)	\
	sugeom$(EXEEXT)		\
	sugeomcsv$(EXEEXT)	\
	suhtmath$(EXEEXT)	\
	sukeycount$(EXEEXT)	\
	sulcthw$(EXEEXT)	\
	sulhead$(EXEEXT)	\
	sunearcsv$(EXEEXT)	\
	sunomstreamers$(EXEEXT)	\
	supaste$(EXEEXT)	\
	suprofcsv$(EXEEXT)	\
	surange$(EXEEXT)	\
	surandhw$(EXEEXT)	\
	susehw$(EXEEXT)		\
	sushw$(EXEEXT)		\
	susmoqcsv$(EXEEXT)	\
	sustrip$(EXEEXT)	\
	sutoolcsv$(EXEEXT)	\
	sutrcount$(EXEEXT)	\
	suutm$(EXEEXT)		\
	suxedit$(EXEEXT)

ifeq ($(AR),lib)
# Windows: build locally then copy
INSTALL:	$(LOCAL_PROGS)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@for %%f in ($(LOCAL_PROGS)) do @copy /Y %%f ..\..\..\..\bin\%%f >nul 2>&1
	@echo. > $@ 2>nul || type nul > $@
	@echo Programs installed in bin directory

# Pattern rule for Windows - build locally
%.exe: %.c
	$(CC) $(CFLAGS) $< /link $(LFLAGS) /OUT:$@
	@echo $* built

else
# Unix version
PROGS =			\
	$B/bhedtopar	\
	$B/su3dchart	\
	$B/suazimuth	\
	$B/suabshw	\
	$B/suahw	\
	$B/suaddhead	\
	$B/subincsv	\
	$B/subinqcsv	\
	$B/subinxxcsv	\
	$B/subinbigcsv	\
	$B/sucdpbin	\
	$B/suchart	\
	$B/suchw	\
	$B/sucliphead	\
	$B/sucountkey	\
	$B/sudumptrace	\
	$B/suedit	\
	$B/suflex2d     \
	$B/sugethw	\
	$B/sugeom	\
	$B/sugeomcsv	\
	$B/suhtmath	\
	$B/sukeycount	\
	$B/sulcthw	\
	$B/sulhead	\
	$B/sunearcsv    \
	$B/sunomstreamers \
	$B/supaste	\
	$B/suprofcsv    \
	$B/surange	\
	$B/surandhw	\
	$B/susehw	\
	$B/sushw	\
	$B/susmoqcsv	\
	$B/sustrip	\
	$B/sutoolcsv	\
	$B/sutrcount	\
	$B/suutm	\
	$B/suxedit	

INSTALL:	$(PROGS)
	@-rm -f INSTALL
	@touch $@

$(PROGS):	$(CTARGET) $D 
	-$(CC) $(CFLAGS) $(@F).c $(LFLAGS) -o $@
	@$(MCHMODLINE)
	@echo $(@F) installed in $B
endif

remake:
	-if exist *.exe del /F /Q *.exe 2>nul
	-if exist INSTALL del /F /Q INSTALL 2>nul
	$(MAKE) INSTALL
	
clean::
	-if exist *.obj del /F /Q *.obj 2>nul
	-if exist *.exe del /F /Q *.exe 2>nul
	-if exist *.ilk del /F /Q *.ilk 2>nul
	-if exist *.pdb del /F /Q *.pdb 2>nul
	-if exist a.out del /F /Q a.out 2>nul
	-if exist junk* del /F /Q junk* 2>nul
	-if exist JUNK* del /F /Q JUNK* 2>nul
	-if exist core del /F /Q core 2>nul
