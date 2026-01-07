# Makefile for Trielas main programs
# Windows/MSVC compatible version

ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

# Tri include path and flags
ifeq ($(AR),lib)
ITRI = $(I)\Triangles
TRICFLAGS = /I"$(I)" /I"$(I_COMPAT)" /I"$(ITRI)" $(OPTC) $(CWP_FLAGS)
EXEEXT = .exe
D = $(L)\libtrielas.lib $(L)\libtriang.lib $(L)\libpar.lib $(L)\libcwp.lib $(L)\libwin32compat.lib
LFLAGS = $(PRELFLAGS) libtrielas.lib libtriang.lib libpar.lib libcwp.lib libwin32compat.lib $(POSTLFLAGS)
else
ITRI = $I/Triangles
TRICFLAGS = -I$I -I$(ITRI) $(OPTC) $(LARGE_FILE_FLAG) $(ENDIANFLAG)
EXEEXT =
LFLAGS = $(PRELFLAGS) -L$L -ltrielas -ltriang -lpar -lcwp -lm $(POSTLFLAGS)
endif

# Program list (gbbeam excluded - already built by tri)
PROG_NAMES = elasyn elacheck elamodel elaray elatriuni raydata

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
	$(CC) $(TRICFLAGS) $< /link $(LFLAGS) /OUT:$@
	@echo $* built

else
# Unix version
PROGS = $(addprefix $B/,$(PROG_NAMES))

INSTALL: $(PROGS)
	@-rm -f INSTALL
	@touch $@

$(PROGS): $(CTARGET)
	$(CC) $(TRICFLAGS) $(@F).c $(LFLAGS) -o $@
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
