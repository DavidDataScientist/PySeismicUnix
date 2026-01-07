# Makefile for psplot library
# DEPRECATED ON WINDOWS

ifeq ($(OS),Windows_NT)

.PHONY: all INSTALL remake clean

all INSTALL remake clean:
	@echo ========================================
	@echo DEPRECATED: psplot library
	@echo ========================================
	@echo PostScript plotting is not supported on Windows.
	@echo This component requires Unix-specific libraries.
	@echo Alternative: Use Python matplotlib or other
	@echo Windows-compatible plotting tools.
	@echo ========================================

else
# Original Unix Makefile content would go here
include $(CWPROOT)/src/Makefile.config
# ... (Unix build rules)
endif
