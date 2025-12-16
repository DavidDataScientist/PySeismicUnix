# Makefile for libtriang (Triangle processing)
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
LIB = $(L)\libtriang.lib
OBJS = check.obj circum.obj colinear.obj create.obj delete.obj distance.obj fixedges.obj inside.obj nearest.obj project.obj readwrite.obj

INSTALL: $(LIB)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@echo. > INSTALL 2>nul || type nul > INSTALL
	@echo libtriang.lib built

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
LIB = $L/libtriang.a

ARCH =			\
	$(LIB)(check.o)		\
	$(LIB)(circum.o)	\
	$(LIB)(colinear.o)	\
	$(LIB)(create.o)	\
	$(LIB)(delete.o)	\
	$(LIB)(distance.o)	\
	$(LIB)(fixedges.o)	\
	$(LIB)(inside.o)	\
	$(LIB)(nearest.o)	\
	$(LIB)(project.o)	\
	$(LIB)(readwrite.o)

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
