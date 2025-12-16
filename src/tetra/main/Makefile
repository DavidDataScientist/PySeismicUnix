# Makefile for tetra main programs
# Windows/MSVC compatible version

ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

# Tetra include path and flags
ifeq ($(AR),lib)
ITETRA = $(I)\Tetra
TETRACFLAGS = /I"$(I)" /I"$(I_COMPAT)" /I"$(ITETRA)" $(OPTC) $(CWP_FLAGS)
EXEEXT = .exe
D = $(L)\libtetra.lib $(L)\libsu.lib $(L)\libpar.lib $(L)\libcwp.lib $(L)\libwin32compat.lib
LFLAGS = $(PRELFLAGS) libtetra.lib libsu.lib libpar.lib libcwp.lib libwin32compat.lib $(POSTLFLAGS)
else
ITETRA = $I/Tetra
TETRACFLAGS = -I$I -I$(ITETRA) $(OPTC) $(LARGE_FILE_FLAG) $(ENDIANFLAG)
EXEEXT =
LFLAGS = $(PRELFLAGS) -L$L -ltetra -lsu -lpar -lcwp -lm $(POSTLFLAGS)
endif

# Program list
PROG_NAMES = tetramod sutetraray

# Build program lists with extensions
LOCAL_PROGS = $(addsuffix $(EXEEXT),$(PROG_NAMES))

ifeq ($(AR),lib)
# Windows: build locally then copy to bin
INSTALL: $(LOCAL_PROGS)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@for %%f in ($(LOCAL_PROGS)) do @copy /Y %%f ..\..\..\bin\%%f >nul 2>&1
	@echo. > INSTALL 2>nul || type nul > INSTALL
	@echo Programs installed in bin directory

# Pattern rule for Windows - build locally
%.exe: %.c
	$(CC) $(TETRACFLAGS) $< /link $(LFLAGS) /OUT:$@
	@echo $* built

else
# Unix version
PROGS = $(addprefix $B/,$(PROG_NAMES))

INSTALL: $(PROGS)
	@-rm -f INSTALL
	@touch $@

$(PROGS): $(CTARGET)
	$(CC) $(TETRACFLAGS) $(@F).c $(LFLAGS) -o $@
	$(MCHMODLINE)
	@echo $(@F) installed in $B
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
	-if exist junk* del /F /Q junk* 2>nul
else
	rm -f junk* core a.out *.o
endif
