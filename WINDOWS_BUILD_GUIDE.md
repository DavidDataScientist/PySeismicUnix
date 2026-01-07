# CWP/SU Windows Build Guide

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

This document describes how to build and use CWP/SU (Seismic Unix) on Windows using Visual Studio 2022 and GNU Make.

**Build Statistics:**
- 312 executables built
- 9 static libraries
- Tested on Windows 10/11 x64

---

## Prerequisites

### Required Software

1. **Visual Studio 2022** (Community Edition or higher)
   - Install "Desktop development with C++" workload
   - Includes MSVC compiler (cl.exe) and linker

2. **GNU Make for Windows**
   - Download from: https://gnuwin32.sourceforge.net/packages/make.htm
   - Install to: `C:\Tools\gnu\bin\make.exe`
   - Or use MSYS2/MinGW make

3. **Python 3.x** (optional, for SU Flow GUI)
   - Required packages: PyQt6

### Directory Structure

```
D:\src\proto\processing-su-main\
├── bin\              # Built executables (312 programs)
├── lib\              # Static libraries (.lib files)
├── include\          # Header files
│   └── win32_compat\ # Windows compatibility headers
├── src\              # Source code
├── test\             # Build scripts
├── zau\scripts\      # Automation scripts
└── zau\              # Custom tools (suflow, PyQt GUI)
```

---

## Quick Start

### 1. Set Up Environment

Open "x64 Native Tools Command Prompt for VS 2022" or run:
```batch
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set PATH=C:\Tools\gnu\bin;%PATH%
```

### 2. Build Core Libraries

```batch
cd D:\src\proto\processing-su-main

REM Build in order:
test\build_libcwp.bat
test\build_libpar.bat
test\build_libsu.bat
test\build_libcomp.bat
```

### 3. Build All SU Programs

```batch
zau\scripts\build_all_su_main.bat
```

### 4. Verify Installation

```batch
bin\surange.exe < src\su\tutorial\data.su
```

---

## Libraries Built

| Library | Description | Location |
|---------|-------------|----------|
| libcwp.lib | Core CWP functions | lib\ |
| libpar.lib | Parameter handling | lib\ |
| libsu.lib | SU core functions | lib\ |
| libcomp.lib | DCT compression | lib\ |
| libwin32compat.lib | Windows compatibility | lib\ |
| libtetra.lib | Tetrahedral processing | lib\ |
| libtriang.lib | Triangle processing | lib\ |
| libtrielas.lib | Elastic triangles | lib\ |
| libreflect.lib | Reflection coefficients | lib\ |

---

## Running SU Programs

### File I/O (Recommended)

```batch
REM Input from file, output to file
bin\surange.exe < input.su
bin\sugain.exe agc=1 < input.su > output.su
```

### Pipelines (Use suflow.exe)

Windows cmd/PowerShell corrupts binary data in pipes. Use `suflow.exe`:

```batch
bin\suflow.exe "suplane | sugain agc=1 | sufilter f=10,20,40,50"
```

### SU Flow GUI

```batch
cd zau\src\pysu
venv\Scripts\activate
python gui\suflow_gui.py
```

---

## Known Issues and Workarounds

### 1. Binary Pipe Corruption

**Issue:** Windows shells corrupt binary SU data in pipes (CR/LF translation).

**Workaround:** Use `suflow.exe` wrapper which uses temporary files:
```batch
bin\suflow.exe "cmd1 | cmd2 | cmd3"
```

### 2. Path Handling

**Issue:** Some programs may have issues with paths containing spaces.

**Workaround:** Install to a path without spaces (e.g., `D:\src\` not `D:\Program Files\`).

### 3. XDR Disabled

**Issue:** XDR (eXternal Data Representation) is disabled on Windows.

**Impact:** Data files are not portable between big-endian and little-endian systems.

**Workaround:** Convert data on the target system or use SEGY format for interchange.

---

## Components Not Built on Windows

### Deprecated (Unix-only graphics)
- **psplot** - PostScript plotting
- **X11 (Xtcwp, xplot)** - X Window graphics
- **Motif (Xmcwp)** - Motif widgets
- **Mesa** - OpenGL (use native Windows OpenGL instead)

### Skipped (Complex dependencies)
- **SFIO/segdread** - SEG-D tape readers (requires complex SFIO library)
- **Complex** - C++ complex numbers (use cwp_complex or std::complex)
- **sup190** - Needs gdbm library
- **segytoseres, Cvt2segd** - Need Unix networking headers

---

## Troubleshooting

### "cl is not recognized"

Run vcvarsall.bat first:
```batch
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

### "make is not recognized"

Add GNU Make to PATH:
```batch
set PATH=C:\Tools\gnu\bin;%PATH%
```

### "Cannot open include file: 'unistd.h'"

Ensure CWPROOT is set and includes win32_compat:
```batch
set CWPROOT=D:\src\proto\processing-su-main
```

### Link errors for popen/pclose

Ensure libwin32compat.lib is linked:
```
LFLAGS = ... libwin32compat.lib ...
```

### "multiple definition of segy tr"

This was fixed by making `tr` an extern declaration in `sugeom.h`.

### Output file is corrupted/wrong size

Use file redirection instead of pipes, or use `suflow.exe` for pipelines.

---

## Build Scripts Reference

| Script | Purpose |
|--------|---------|
| test\build_libcwp.bat | Build libcwp.lib |
| test\build_libpar.bat | Build libpar.lib |
| test\build_libsu.bat | Build libsu.lib |
| test\build_libcomp.bat | Build libcomp.lib |
| test\build_cwp_main.bat | Build CWP utilities |
| test\build_tetra.bat | Build tetra components |
| test\build_tri.bat | Build triangle components |
| test\build_trielas.bat | Build elastic triangle |
| test\build_refl.bat | Build reflection library |
| test\build_third_party.bat | Build third-party tools |
| zau\scripts\build_all_su_main.bat | Build all SU programs |
| zau\scripts\generate_su_makefiles.ps1 | Generate Makefiles |

---

## Additional Resources

### Windows Compatibility Documentation

- **[include/README.md](../include/README.md)** - Header file modifications for Windows
  - Changes to `cwp.h` and `su_xdr.h`
  - Windows compatibility fixes and implementation details

- **[include/win32_compat/README.md](../include/win32_compat/README.md)** - Windows compatibility layer
  - Complete POSIX compatibility implementation
  - File operations, directory handling, path conversion
  - Detailed technical documentation

### Build Scripts

- **[scripts/README.md](../scripts/README.md)** - Build scripts reference
  - All build, clean, and distribution scripts
  - Usage and troubleshooting

## Contact & Resources

- **CWP/SU Homepage:** https://www.cwp.mines.edu/cwpcodes/
- **Seismic Unix Wiki:** https://wiki.seismic-unix.org/

---

*Document generated: December 2024*
*Windows port by: AI Assistant with user guidance*

