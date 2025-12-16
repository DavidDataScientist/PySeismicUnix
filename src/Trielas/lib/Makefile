# Makefile for libtrielas (Elastic triangle)
# Windows/MSVC compatible version

ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

# Tri include path
ifeq ($(AR),lib)
ITRI = $(I)\Triangles
TRICFLAGS = /I"$(I)" /I"$(I_COMPAT)" /I"$(ITRI)" $(OPTC) $(CWP_FLAGS)
LIB = $(L)\libtrielas.lib
OBJS = findPiso.obj findSiso.obj findqSH.obj rottensh.obj rottensor.obj rotvector.obj rt_SHa_real.obj rt_ani_real.obj rt_iso_cmplx.obj rt_iso_real.obj solveForSlowqP.obj solveForSlowqSV.obj

INSTALL: $(LIB)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@echo. > INSTALL 2>nul || type nul > INSTALL
	@echo libtrielas.lib built

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$@ $(OBJS)
	@echo $(LIB) created

%.obj: %.c
	$(CC) $(TRICFLAGS) /c $<

else
# Unix version
ITRI = $I/Triangles
TRICFLAGS = -I$I -I$(ITRI) $(OPTC) $(LARGE_FILE_FLAG) $(ENDIANFLAG)
D = $(ITRI)/tri.h $(ITRI)/triP.h
LIB = $L/libtrielas.a

ARCH =				\
	$(LIB)(findPiso.o)	\
	$(LIB)(findSiso.o)	\
	$(LIB)(findqSH.o)	\
	$(LIB)(rottensh.o)	\
	$(LIB)(rottensor.o)	\
	$(LIB)(rotvector.o)	\
	$(LIB)(rt_SHa_real.o)	\
	$(LIB)(rt_ani_real.o)	\
	$(LIB)(rt_iso_cmplx.o)	\
	$(LIB)(rt_iso_real.o)	\
	$(LIB)(solveForSlowqP.o)\
	$(LIB)(solveForSlowqSV.o)

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

list:
	$(AR) tv $(LIB)

clean:
ifeq ($(AR),lib)
	-if exist *.obj del /F /Q *.obj 2>nul
	-if exist *.pdb del /F /Q *.pdb 2>nul
	-if exist junk* del /F /Q junk* 2>nul
else
	rm -f junk* core a.out *.o
endif

.PRECIOUS: $(LIB)
