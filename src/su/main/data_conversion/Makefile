# Makefile for ...su/main/data_conversion

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
	dt1tosu$(EXEEXT)	\
	segyclean$(EXEEXT)	\
	segyhdrmod$(EXEEXT)	\
	segyhdrs$(EXEEXT)	\
	segyread$(EXEEXT)	\
	segywrite$(EXEEXT)	\
	segyscan$(EXEEXT)	\
	setbhed$(EXEEXT)	\
	suascii$(EXEEXT)	\
	suintvel$(EXEEXT)	\
	suoldtonew$(EXEEXT)	\
	sustkvel$(EXEEXT)	\
	suswapbytes$(EXEEXT)	\
	swapbhed$(EXEEXT)

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
	$B/dt1tosu	\
	$B/segyclean	\
	$B/segyhdrmod	\
	$B/segyhdrs	\
	$B/segyread	\
	$B/segywrite	\
	$B/segyscan	\
	$B/setbhed	\
	$B/suascii	\
	$B/suintvel	\
	$B/suoldtonew	\
	$B/sustkvel	\
	$B/suswapbytes	\
	$B/swapbhed

INSTALL:	$(PROGS)
	@-rm -f INSTALL
	@touch $@

$(PROGS):	$(CTARGET) $(D)
	-$(CC) $(CFLAGS) $(@F).c $(LFLAGS) -o $@
	@$(MCHMODLINE)
	@echo $(@F) installed in $(B)
endif

remake:
ifeq ($(AR),lib)
	-if exist *.exe del /F /Q *.exe 2>nul
	-if exist INSTALL del /F /Q INSTALL 2>nul
else
	-rm -f $(PROGS) INSTALL
endif
	$(MAKE)

clean::
ifeq ($(AR),lib)
	-if exist *.o del /F /Q *.o 2>nul
	-if exist *.obj del /F /Q *.obj 2>nul
	-if exist *.exe del /F /Q *.exe 2>nul
	-if exist a.out del /F /Q a.out 2>nul
	-if exist junk* del /F /Q junk* 2>nul
	-if exist JUNK* del /F /Q JUNK* 2>nul
	-if exist core del /F /Q core 2>nul
else
	rm -f a.out junk* JUNK* core
endif
