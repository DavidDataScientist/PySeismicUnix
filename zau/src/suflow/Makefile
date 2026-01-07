# Makefile for suflow - Windows binary pipe wrapper
#
# Build with: make
# Requires: Visual Studio 2022 (cl.exe in PATH)

CC = cl
CFLAGS = /nologo /W3 /O2 /MD
LDFLAGS = /nologo

TARGET = suflow.exe
SOURCES = suflow.c
OBJECTS = suflow.obj

# Installation directory (relative to project root)
BINDIR = ..\..\..\bin

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) /Fe:$(TARGET) $(OBJECTS)

suflow.obj: suflow.c
	$(CC) $(CFLAGS) /c suflow.c

install: $(TARGET)
	@if not exist $(BINDIR) mkdir $(BINDIR)
	copy /Y $(TARGET) $(BINDIR)\$(TARGET)
	@echo "suflow.exe installed to $(BINDIR)"

clean:
	-del /Q $(TARGET) $(OBJECTS) *.pdb *.ilk 2>nul

.PHONY: all install clean

