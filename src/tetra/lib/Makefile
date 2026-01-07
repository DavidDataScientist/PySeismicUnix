# Makefile for libtetra (Tetrahedral processing)
# Windows/MSVC compatible version

ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

# Tetra include path
ifeq ($(AR),lib)
ITETRA = $(I)\Tetra
TETRACFLAGS = /I"$(I)" /I"$(I_COMPAT)" /I"$(ITETRA)" $(OPTC) $(CWP_FLAGS)
LIB = $(L)\libtetra.lib
OBJS = tetra.obj

INSTALL: $(LIB)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@echo. > INSTALL 2>nul || type nul > INSTALL
	@echo libtetra.lib built

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$@ $(OBJS)
	@echo $(LIB) created

%.obj: %.c
	$(CC) $(TETRACFLAGS) /c $<

else
# Unix version
ITETRA = $I/Tetra
TETRACFLAGS = -I$I -I$(ITETRA) $(OPTC) $(LARGE_FILE_FLAG) $(ENDIANFLAG)
D = $(ITETRA)/tetra.h
LIB = $L/libtetra.a

ARCH = $(LIB)(tetra.o)

INSTALL: $(LIB)
	@-rm -f INSTALL
	@touch $@

$(ARCH): $D

$(LIB): $(ARCH)
	$(RANLIB) $(LIB)
endif

remake:
ifeq ($(AR),lib)
	-if exist $(LIB) del /F /Q $(LIB) 2>nul
	-if exist *.obj del /F /Q *.obj 2>nul
	-if exist INSTALL del /F /Q INSTALL 2>nul
else
	@-rm -f $(LIB) INSTALL
endif
	$(MAKE)

clean:
ifeq ($(AR),lib)
	-if exist *.obj del /F /Q *.obj 2>nul
	-if exist *.pdb del /F /Q *.pdb 2>nul
	-if exist junk* del /F /Q junk* 2>nul
else
	rm -f junk* core a.out *.o
endif

.PRECIOUS: $(LIB)
