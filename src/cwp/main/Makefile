# Makefile for ...cwp/main
# Just say "make"

# Use backslashes for Windows paths
ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

# Executable extension - .exe on Windows, nothing on Unix
ifeq ($(AR),lib)
EXEEXT = .exe
# For MSVC, use /link to separate compile and link flags
# Library should be specified after /link, not in LFLAGS
LFLAGS = $(PRELFLAGS) $(POSTLFLAGS)
LIBRARIES = libcwp.lib
else
EXEEXT =
LFLAGS = $(PRELFLAGS) -L$L -lcwp $(POSTLFLAGS)
LIBRARIES =
endif

# Build locally first, then copy to bin directory
LOCAL_PROGS =			\
	ctrlstrip$(EXEEXT)	\
	conjtest$(EXEEXT)	\
	downfort$(EXEEXT)	\
	fcat$(EXEEXT)	\
	isatty$(EXEEXT)	\
	maxints$(EXEEXT)	\
	pause$(EXEEXT)	\
	t$(EXEEXT)		\
	upfort$(EXEEXT)

# Use relative path to avoid drive letter colon issues
ifeq ($(AR),lib)
BIN_DIR = ../../bin
PROGS =			\
	$(BIN_DIR)/ctrlstrip$(EXEEXT)	\
	$(BIN_DIR)/conjtest$(EXEEXT)	\
	$(BIN_DIR)/downfort$(EXEEXT)	\
	$(BIN_DIR)/fcat$(EXEEXT)	\
	$(BIN_DIR)/isatty$(EXEEXT)	\
	$(BIN_DIR)/maxints$(EXEEXT)	\
	$(BIN_DIR)/pause$(EXEEXT)	\
	$(BIN_DIR)/t$(EXEEXT)		\
	$(BIN_DIR)/upfort$(EXEEXT)
else
PROGS =			\
	$B/ctrlstrip$(EXEEXT)	\
	$B/conjtest$(EXEEXT)	\
	$B/downfort$(EXEEXT)	\
	$B/fcat$(EXEEXT)	\
	$B/isatty$(EXEEXT)	\
	$B/maxints$(EXEEXT)	\
	$B/pause$(EXEEXT)	\
	$B/t$(EXEEXT)		\
	$B/upfort$(EXEEXT)
endif


INSTALL:	$(LOCAL_PROGS)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@copy /Y ctrlstrip$(EXEEXT) ..\..\..\bin\ctrlstrip$(EXEEXT) >nul 2>&1
	@copy /Y conjtest$(EXEEXT) ..\..\..\bin\conjtest$(EXEEXT) >nul 2>&1
	@copy /Y downfort$(EXEEXT) ..\..\..\bin\downfort$(EXEEXT) >nul 2>&1
	@copy /Y fcat$(EXEEXT) ..\..\..\bin\fcat$(EXEEXT) >nul 2>&1
	@copy /Y isatty$(EXEEXT) ..\..\..\bin\isatty$(EXEEXT) >nul 2>&1
	@copy /Y maxints$(EXEEXT) ..\..\..\bin\maxints$(EXEEXT) >nul 2>&1
	@copy /Y pause$(EXEEXT) ..\..\..\bin\pause$(EXEEXT) >nul 2>&1
	@copy /Y t$(EXEEXT) ..\..\..\bin\t$(EXEEXT) >nul 2>&1
	@copy /Y upfort$(EXEEXT) ..\..\..\bin\upfort$(EXEEXT) >nul 2>&1
	@echo. > $@ 2>nul || type nul > $@

# Pattern rule for building programs locally
ifeq ($(AR),lib)
# Windows: build locally first, then copy to bin directory
# Build to local directory to avoid path issues
# Don't use $(CTARGET) as it contains $B which has a colon (D:)
ctrlstrip$(EXEEXT):	ctrlstrip.c
	$(CC) $(CFLAGS) ctrlstrip.c /link $(LFLAGS) $(LIBRARIES) /OUT:ctrlstrip$(EXEEXT)
	@echo ctrlstrip built

conjtest$(EXEEXT):	conjtest.c
	$(CC) $(CFLAGS) conjtest.c /link $(LFLAGS) $(LIBRARIES) /OUT:conjtest$(EXEEXT)
	@echo conjtest built

downfort$(EXEEXT):	downfort.c
	$(CC) $(CFLAGS) downfort.c /link $(LFLAGS) $(LIBRARIES) /OUT:downfort$(EXEEXT)
	@echo downfort built

fcat$(EXEEXT):	fcat.c
	$(CC) $(CFLAGS) fcat.c /link $(LFLAGS) $(LIBRARIES) /OUT:fcat$(EXEEXT)
	@echo fcat built

isatty$(EXEEXT):	isatty.c
	$(CC) $(CFLAGS) isatty.c /link $(LFLAGS) $(LIBRARIES) /OUT:isatty$(EXEEXT)
	@echo isatty built

maxints$(EXEEXT):	maxints.c
	$(CC) $(CFLAGS) maxints.c /link $(LFLAGS) $(LIBRARIES) /OUT:maxints$(EXEEXT)
	@echo maxints built

pause$(EXEEXT):	pause.c
	$(CC) $(CFLAGS) pause.c /link $(LFLAGS) $(LIBRARIES) /OUT:pause$(EXEEXT)
	@echo pause built

t$(EXEEXT):	t.c
	$(CC) $(CFLAGS) t.c /link $(LFLAGS) $(LIBRARIES) /OUT:t$(EXEEXT)
	@echo t built

upfort$(EXEEXT):	upfort.c
	$(CC) $(CFLAGS) upfort.c /link $(LFLAGS) $(LIBRARIES) /OUT:upfort$(EXEEXT)
	@echo upfort built
else
# Unix version
$(PROGS):	$(CTARGET) $L/libcwp.a
	$(CC) $(CFLAGS) $(@F).c $(LFLAGS) -o $@
	$(MCHMODLINE)
	@echo $(@F) installed in $B
endif

remake:
	@-if exist $(LOCAL_PROGS) del /F /Q $(LOCAL_PROGS) 2>nul
	@-if exist $(PROGS) del /F /Q $(PROGS) 2>nul
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@$(MAKE) INSTALL

clean::
	-if exist *.o del /F /Q *.o 2>nul
	-if exist *.a del /F /Q *.a 2>nul
	-if exist *.obj del /F /Q *.obj 2>nul
	-if exist *.exe del /F /Q *.exe 2>nul
	-if exist junk* del /F /Q junk* 2>nul
