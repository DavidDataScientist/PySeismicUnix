# Makefile for libreflect (Reflection coefficients)
# Windows/MSVC compatible version

ifeq ($(OS),Windows_NT)
include $(CWPROOT)\src\Makefile.config
else
include $(CWPROOT)/src/Makefile.config
endif

# Refl include path
ifeq ($(AR),lib)
IREFL = $(I)\Refl
REFLCFLAGS = /I"$(I)" /I"$(I_COMPAT)" /I"$(IREFL)" $(OPTC) $(CWP_FLAGS)
LIB = $(L)\libreflect.lib
OBJS = compute.obj convolve.obj auxil.obj psvreflect.obj shreflect.obj synthetic.obj

INSTALL: $(LIB)
	@-if exist INSTALL del /F /Q INSTALL 2>nul
	@echo. > INSTALL 2>nul || type nul > INSTALL
	@echo libreflect.lib built

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$@ $(OBJS)
	@echo $(LIB) created

%.obj: %.c
	$(CC) $(REFLCFLAGS) /c $<

else
# Unix version
LIB = $L/libreflect.a

ARCH =				\
	$(LIB)(compute.o)	\
	$(LIB)(convolve.o)	\
	$(LIB)(auxil.o)		\
	$(LIB)(psvreflect.o)	\
	$(LIB)(shreflect.o)	\
	$(LIB)(synthetic.o)

INSTALL: $(LIB)
	@-rm -f INSTALL
	@touch $@

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
