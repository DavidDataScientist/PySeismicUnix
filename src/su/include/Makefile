# Description file for SU includes
# Note that header.h and hdr.h are CREATED, not retrieved
#
# $Author: jkc $
# $Revision: 1.9 $ ; $Date: 88/05/15 06:37:27 $

include $(CWPROOT)/src/Makefile.config


INCLUDE = 	$I/segy.h	\
		$I/segd.h	\
		$I/hdr.h	\
		$I/header.h	\
		$I/su.h		\
		$I/tapesegy.h   \
		$I/tapebhdr.h  \
		$I/tapehdr.h   \
		$I/bheader.h   \
		$I/bhdr.h    \
		$I/sugeom.h    \
		$I/qdefine.h   \
		$I/headcase.h  \
		$I/linterpd.h  \
		$I/bilinear.h  \
		$I/readkfile.h \
		$I/gridread.h  \
		$I/gridxy.h    \
		$I/su_xdr.h

INSTALL	: header.h hdr.h mkxdrhdrsub mkxdrbhdrsub $(INCLUDE) ../lib/xdrhdrsub.c ../lib/xdrbhdrsub.c 
	@echo. > $@ 2>nul || type nul > $@

header.h	: mkheader
	./mkheader >header.h
mkheader	: mkheader.c prehdr.h
	$(CC) $(CFLAGS) mkheader.c -o mkheader

hdr.h	:mkhdr
	./mkhdr >hdr.h

mkhdr	:mkhdr.c offsets.h prehdr.h
	$(CC) $(CFLAGS) mkhdr.c -o mkhdr

offsets.h	:mkoffs.sh segy.h mkoffs.ps1
	@if exist mkoffs.ps1 ( \
		$(CPP) $(CFLAGS) segy.h | powershell -ExecutionPolicy Bypass -File mkoffs.ps1 >offsets.h \
	) else ( \
		$(CPP) $(CFLAGS) segy.h | ./mkoffs.sh >offsets.h \
	)
prehdr.h	:mkprehdr.sh segy.h mkprehdr.ps1
	@if exist mkprehdr.ps1 ( \
		$(CPP) $(CFLAGS) segy.h | powershell -ExecutionPolicy Bypass -File mkprehdr.ps1 >prehdr.h \
	) else ( \
		$(CPP) $(CFLAGS) segy.h | ./mkprehdr.sh >prehdr.h \
	)

$(INCLUDE):	$(ITARGET)
	@-if exist $@ del /F /Q $@ 2>nul
	@copy $? $@ >nul 2>&1
	@$(ICHMODLINE)
	@echo $(@F) installed in $I

mkxdrhdrsub	:hdr.h su.h mkxdrhdrsub.c
	$(CC) $(CFLAGS) mkxdrhdrsub.c -o mkxdrhdrsub

../lib/xdrhdrsub.c	:mkxdrhdrsub.c
	./mkxdrhdrsub > ../lib/xdrhdrsub.c

mkxdrbhdrsub	:bhdr.h su.h mkxdrbhdrsub.c
	$(CC) $(CFLAGS) mkxdrbhdrsub.c -o mkxdrbhdrsub

../lib/xdrbhdrsub.c	:mkxdrbhdrsub.c
	./mkxdrbhdrsub > ../lib/xdrbhdrsub.c

clean	::
	-if exist mkheader del /F /Q mkheader 2>nul
	-if exist mkhdr del /F /Q mkhdr 2>nul
	-if exist mkxdrhdrsub del /F /Q mkxdrhdrsub 2>nul
	-if exist mkxdrbhdrsub del /F /Q mkxdrbhdrsub 2>nul
	-if exist offsets.h del /F /Q offsets.h 2>nul
	-if exist prehdr.h del /F /Q prehdr.h 2>nul
	-if exist *.o del /F /Q *.o 2>nul
	-if exist core del /F /Q core 2>nul

remake	:
	@$(MAKE) clean
	@for %%f in (*.h) do @echo. > %%f 2>nul || type nul > %%f
	@-if exist $(INCLUDE) del /F /Q $(INCLUDE) 2>nul
	@-if exist ../lib/xdrbhdrsub.c del /F /Q ../lib/xdrbhdrsub.c 2>nul
	@-if exist ../lib/xdrhdrsub.c del /F /Q ../lib/xdrhdrsub.c 2>nul
	@$(MAKE) ROOT=$(ROOT) INSTALL
