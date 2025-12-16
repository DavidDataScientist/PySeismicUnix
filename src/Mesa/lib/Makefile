# Makefile for Mesa library (OpenGL)
# DEPRECATED ON WINDOWS

ifeq ($(OS),Windows_NT)

.PHONY: all INSTALL remake clean

all INSTALL remake clean:
	@echo ========================================
	@echo DEPRECATED: Mesa library - OpenGL
	@echo ========================================
	@echo Mesa 3D graphics is not built on Windows.
	@echo Windows has native OpenGL via GPU drivers.
	@echo ========================================

else
# Original Unix Makefile content would go here
include $(CWPROOT)/src/Makefile.config
# ... (Unix build rules)
endif
