# Makefile for libcomp (DCT compression library)
# Windows/MSVC compatible version

ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

# Include path for comp headers
I_COMP = $(CWPROOT)/src/comp/dct/include

ifeq ($(AR),lib)
# Windows MSVC
DCTCFLAGS = /I"$(I)" /I"$(I_COMPAT)" /I"$(I_COMP)" $(OPTC) $(CWP_FLAGS)
LIB = $(L)\libcomp.lib
OBJS = buffalloc.obj huffman.obj pcoding.obj quant.obj rle.obj \
       dct1.obj dct2.obj dctalloc.obj getfilter.obj lct1.obj \
       lpred.obj wavepack1.obj wavepack2.obj wavetrans1.obj wavetrans2.obj

INSTALL: $(LIB)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@echo. > INSTALL 2>nul || type nul > INSTALL
	@echo libcomp.lib built

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$@ $(OBJS)
	@echo $(LIB) created

%.obj: %.c
	$(CC) $(DCTCFLAGS) /c $<

else
# Unix
DCTCFLAGS = -I$(I) -I$(I_COMP) $(OPTC) $(CWP_FLAGS)
LIB = $L/libcomp.a

ARCH =				\
	$(LIB)(buffalloc.o)	\
	$(LIB)(huffman.o)	\
	$(LIB)(pcoding.o)	\
	$(LIB)(quant.o)		\
	$(LIB)(rle.o)		\
	$(LIB)(dct1.o)		\
	$(LIB)(dct2.o)		\
	$(LIB)(dctalloc.o)	\
	$(LIB)(getfilter.o)	\
	$(LIB)(lct1.o)		\
	$(LIB)(lpred.o)		\
	$(LIB)(wavepack1.o)	\
	$(LIB)(wavepack2.o)	\
	$(LIB)(wavetrans1.o)	\
	$(LIB)(wavetrans2.o)

INSTALL:	$(LIB)
	@-rm -f INSTALL
	@touch $@

$(ARCH)	: $D

$(LIB)	:	$(ARCH)
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
