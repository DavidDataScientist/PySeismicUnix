# Makefile for SFIO library
# SKIPPED ON WINDOWS

ifeq ($(OS),Windows_NT)

.PHONY: all INSTALL remake clean

all INSTALL remake clean:
	@echo ========================================
	@echo SKIPPED: SFIO Library
	@echo ========================================
	@echo SFIO (Safe/Fast I/O) is not built on Windows.
	@echo Reason: Complex Unix-specific library (~80 files)
	@echo         requiring pthreads, mmap, iffe build system.
	@echo Programs affected: segdread, segbread (SEG-D tape readers)
	@echo Workaround: Convert SEG-D on Unix, use SU/SEGY on Windows.
	@echo ========================================

else
# Original Unix build
include $(CWPROOT)/src/Makefile.config

DIRS = src/lib/sfio src/lib/vthread lib include main

INSTALL:
	for i in $(DIRS) ; do \
	  ( cd $$i ; $(MAKE) ) ; \
	done
	@-rm -f INSTALL
	@touch $@
endif
