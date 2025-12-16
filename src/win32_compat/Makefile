# Makefile for win32_compat library
# Windows compatibility functions for CWP/SU

ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

# Library name
ifeq ($(AR),lib)
LIB = $(L)\libwin32compat.lib
OBJS = win32_compat.obj path.obj
else
LIB = $(L)/libwin32compat.a
OBJS = win32_compat.o path.o
endif

INSTALL: $(LIB)
	@echo. > $@ 2>nul || type nul > $@

ifeq ($(AR),lib)
# Windows: compile to .obj, then create library
$(LIB): $(OBJS)
	$(AR) /OUT:$@ $(OBJS)
	@echo libwin32compat.lib created

%.obj: %.c
	$(CC) $(CFLAGS) /c $< /Fo$@

else
# Unix: use ar
$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	$(RANLIB) $@
	@echo libwin32compat.a created

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
endif

clean::
	-if exist *.obj del /F /Q *.obj 2>nul
	-if exist *.o del /F /Q *.o 2>nul
	-if exist INSTALL del /F /Q INSTALL 2>nul

