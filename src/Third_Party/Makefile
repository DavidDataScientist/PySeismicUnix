# Makefile for Third Party components
# Windows/MSVC compatible version

ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

ifeq ($(AR),lib)
EXEEXT = .exe
D = $(L)\libsu.lib $(L)\libpar.lib $(L)\libcwp.lib $(L)\libwin32compat.lib
LFLAGS = $(PRELFLAGS) libsu.lib libpar.lib libcwp.lib libwin32compat.lib $(POSTLFLAGS)
else
EXEEXT =
LFLAGS = $(PRELFLAGS) -L$L -lsu -lpar -lcwp -lm $(POSTLFLAGS)
endif

# Programs that can build on Windows
# bison2su - standalone (works)
# seg2segy - standalone (needs testing)
# sucoher - needs SU libs + Bool type fix

PROG_NAMES = bison2su

LOCAL_PROGS = $(addsuffix $(EXEEXT),$(PROG_NAMES))

ifeq ($(AR),lib)
INSTALL: $(LOCAL_PROGS)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@for %%f in ($(LOCAL_PROGS)) do @copy /Y %%f ..\..\bin\%%f >nul 2>&1
	@echo. > INSTALL 2>nul || type nul > INSTALL
	@echo Third Party programs installed in bin directory

bison2su.exe: bison2su\bison2su.c
	$(CC) $(CFLAGS) $< /link /OUT:$@
	@echo bison2su built

else
# Unix version
PROGS = $(addprefix $B/,$(PROG_NAMES))

INSTALL: $(PROGS)
	@-rm -f INSTALL
	@touch $@

$B/bison2su: bison2su/bison2su.c
	$(CC) $(CFLAGS) $< -o $@ -lm
	@echo bison2su installed in $B
endif

remake:
ifeq ($(AR),lib)
	-if exist *.exe del /F /Q *.exe 2>nul
	-if exist INSTALL del /F /Q INSTALL 2>nul
else
	@-rm -f $(PROGS) INSTALL
endif
	$(MAKE)

clean:
ifeq ($(AR),lib)
	-if exist *.obj del /F /Q *.obj 2>nul
	-if exist *.exe del /F /Q *.exe 2>nul
	-if exist *.lib del /F /Q *.lib 2>nul
	-if exist *.exp del /F /Q *.exp 2>nul
	-if exist *.pdb del /F /Q *.pdb 2>nul
	-if exist *.ilk del /F /Q *.ilk 2>nul
else
	rm -f junk* core a.out *.o
endif

