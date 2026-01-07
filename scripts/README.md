# Build Scripts

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

This directory contains the main build scripts for PySeismicUnix (CWP/SU for Windows). These scripts provide a streamlined interface for building, cleaning, and packaging the project.

## Core Build Scripts

### `build.bat`

**Purpose**: Build all libraries and SU main programs (without Fortran)

**Usage**:
```cmd
cd scripts
build.bat
```

**What it builds**:
- **Libraries**: libcwp, libpar, libsu, libwin32compat
- **SU Programs**: All SU main utilities (amplitudes, filters, transforms, etc.)

**What it does NOT build**:
- Fortran components (use `build_fortran.bat` for those)

**Requirements**:
- Visual Studio 2022 (Community, Professional, or Enterprise)
- GNU Make (expected at `C:\Tools\gnu\bin`)
- MSVC compiler (cl.exe)

**Output**:
- Libraries: `lib/*.lib`
- Executables: `bin/*.exe`

---

### `wipe.bat`

**Purpose**: Remove all final build outputs (libraries and executables)

**Usage**:
```cmd
cd scripts
wipe.bat
wipe.bat --dist    # Also remove distribution directory
```

**What it removes**:
- All `.lib` and `.a` files from `lib/` directory
- All `.exe` files from `bin/` directory
- (Optional) `dist/` directory if `--dist` flag is used

**WARNING**: This permanently deletes all built libraries and executables!

**Use cases**:
- Complete clean slate before rebuilding
- Free up disk space
- Remove all build outputs for a fresh start

**Note**: This is more aggressive than `clean.bat`. Use `wipe.bat` to remove final outputs, then `clean.bat` to remove intermediate artifacts.

---

### `clean.bat`

**Purpose**: Clean all build artifacts from the project

**Usage**:
```cmd
cd scripts
clean.bat
```

**What it cleans**:
- Object files (`.obj`) from all directories
- Export files (`.exp`) from `bin/` and `lib/` directories
- Library object files from `lib/` directory
- Build artifacts from all SU main subdirectories
- Build artifacts from library directories

**Note**: This does NOT delete the final `.lib` files in `lib/` or `.exe` files in `bin/` - it only removes intermediate build artifacts. Use `wipe.bat` to remove final outputs (`.lib`, `.a`, and `.exe` files).

---

### `build_suflow.bat`

**Purpose**: Build suflow.exe - Windows binary pipe wrapper

**Usage**:
```cmd
cd scripts
build_suflow.bat
```

**What it builds**:
- **suflow.exe**: Windows binary pipe wrapper for SU pipelines
  - Enables reliable binary data transfer between SU programs
  - Solves Windows pipe corruption issues

**Requirements**:
- Visual Studio 2022 (Community, Professional, or Enterprise)
- MSVC compiler (cl.exe)

**Output**:
- Executable: `bin/suflow.exe`

**Note**: This is a standalone utility that doesn't require GNU Make. It's built directly with MSVC.

---

### `build_fortran.bat`

**Purpose**: Build all Fortran components

**Usage**:
```cmd
cd scripts
build_fortran.bat
```

**What it builds**:
- **Cshot**: Shot gather processing
- **Cwell**: Well log processing
- **Triso**: 3D processing
- **Vplusz**: Velocity analysis
- **Vzest**: Velocity estimation

**Requirements**:
- Visual Studio 2022
- Intel oneAPI Fortran Compiler (`ifx.exe` or `ifort.exe`)
- GNU Make
- MSVC runtime libraries

**Output**:
- Fortran executables: `bin/*.exe` (Fortran programs)

**Note**: Run `verify_fortran.bat` first to check compiler setup.

---

### `build_release.bat`

**Purpose**: Build everything and create a distribution package

**Usage**:
```cmd
cd scripts
build_release.bat
```

**What it does**:
1. Builds all libraries (via `build.bat` logic)
2. Builds all SU programs (via `build.bat` logic)
3. Creates distribution directory structure
4. Copies all binaries, libraries, headers, and documentation
5. Creates a ZIP archive

**Output**:
- Distribution folder: `dist/cwpsu-windows-x64/`
- ZIP archive: `dist/cwpsu-windows-x64.zip`

**Distribution Contents**:
- `bin/` - All executables
- `lib/` - All static libraries
- `include/` - Header files
- `doc/` - Documentation
- `man/` - Unix man pages
- `samples/` - Sample SU data files
- `gui/` - PyQt6 SU Flow GUI

---

### `verify_fortran.bat`

**Purpose**: Verify Fortran compiler setup

**Usage**:
```cmd
cd scripts
verify_fortran.bat
```

**What it checks**:
- Intel oneAPI Fortran Compiler (`ifx.exe` - recommended)
- Legacy Intel Fortran Compiler (`ifort.exe` - fallback)
- Compiler version information
- Environment setup

**Output**:
- Status messages indicating which compiler is found
- Version information if compiler is available
- Error messages if no compiler is found

**Note**: Run this before `build_fortran.bat` to ensure your Fortran environment is properly configured.

---

## Build Workflows

### Standard Build (No Fortran)

```cmd
cd scripts
build.bat
```

This builds all C/C++ components (libraries and SU programs).

### Build suflow.exe

```cmd
cd scripts
build_suflow.bat
```

This builds the suflow.exe binary pipe wrapper utility.

### Clean Build

```cmd
cd scripts
clean.bat
build.bat
```

Clean all artifacts, then rebuild from scratch.

### Complete Wipe and Rebuild

```cmd
cd scripts
wipe.bat
clean.bat
build.bat
```

Remove all final outputs, clean intermediate artifacts, then rebuild from scratch.

### Wipe Including Distribution

```cmd
cd scripts
wipe.bat --dist
```

Remove all build outputs and distribution packages.

### Full Build (Including Fortran)

```cmd
cd scripts
build.bat
build_fortran.bat
```

Build all C/C++ components, then build Fortran components.

### Release Build

```cmd
cd scripts
build_release.bat
```

Build everything and create a distribution package.

### Verify Fortran Setup

```cmd
cd scripts
verify_fortran.bat
```

Check if Fortran compiler is properly configured.

---

## Requirements

### Essential Tools

1. **Visual Studio 2022**
   - Community, Professional, or Enterprise edition
   - C++ development tools
   - MSVC compiler (cl.exe)

2. **GNU Make**
   - Expected location: `C:\Tools\gnu\bin\make.exe`
   - Can be installed via MSYS2, MinGW, or standalone

3. **Python 3.13.x** (for GUI and plugins)
   - Required for SU Flow GUI
   - Required for Python plugins

### Optional Tools

4. **Intel oneAPI Fortran Compiler** (for Fortran components)
   - Recommended: `ifx.exe` (newer compiler)
   - Legacy: `ifort.exe` (older compiler)
   - Required only if building Fortran components

---

## Environment Setup

### Visual Studio 2022

The scripts automatically detect Visual Studio 2022 in these locations:
- `%ProgramFiles%\Microsoft Visual Studio\2022\Community`
- `%ProgramFiles%\Microsoft Visual Studio\2022\Professional`
- `%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise`
- `%ProgramFiles(x86)%\Microsoft Visual Studio\2022\*` (32-bit installations)

### GNU Make

Ensure GNU Make is in your PATH or at `C:\Tools\gnu\bin\make.exe`.

### Fortran Compiler

For Fortran builds, ensure Intel oneAPI is installed and `ifx.exe` (or `ifort.exe`) is in your PATH.

---

## Build Output

### Libraries

Built libraries are placed in `lib/`:
- `libcwp.lib` - CWP library
- `libpar.lib` - Parameter parsing library
- `libsu.lib` - SU library
- `libwin32compat.lib` - Windows compatibility library

### Executables

Built executables are placed in `bin/`:
- SU programs: `sugain.exe`, `sufilter.exe`, `surange.exe`, etc.
- Fortran programs: `cshot.exe`, `cwell.exe`, `triso.exe`, etc.
- Utilities: `suflow.exe`, `pysurun.exe`, etc.

### Distribution

Distribution packages are created in `dist/`:
- `dist/cwpsu-windows-x64/` - Unpacked distribution
- `dist/cwpsu-windows-x64.zip` - ZIP archive

---

## Troubleshooting

### Build Fails: "Visual Studio 2022 not found"

**Solution**: 
- Ensure Visual Studio 2022 is installed
- Check that `vcvarsall.bat` exists in the expected location
- Try running from Visual Studio Developer Command Prompt

### Build Fails: "GNU Make not found"

**Solution**:
- Install GNU Make
- Ensure it's at `C:\Tools\gnu\bin\make.exe`
- Or add it to your PATH

### Build Fails: "Fortran compiler not found"

**Solution**:
- Install Intel oneAPI Fortran Compiler
- Run `verify_fortran.bat` to check setup
- Ensure `ifx.exe` or `ifort.exe` is in PATH

### Some Programs Fail to Build

**Solution**:
- Check error messages in the build output
- Ensure all libraries built successfully first
- Try a clean build: `clean.bat` then `build.bat`
- For a complete fresh start: `wipe.bat` then `clean.bat` then `build.bat`

### Distribution Creation Fails

**Solution**:
- Ensure PowerShell is available (for ZIP creation)
- Check that all components built successfully
- Verify disk space is available

---

## Script Organization

### Active Scripts

These scripts are actively maintained and used:

- `build.bat` - Main build script
- `clean.bat` - Clean script
- `build_suflow.bat` - suflow.exe build script
- `build_fortran.bat` - Fortran build script
- `build_release.bat` - Release build script
- `verify_fortran.bat` - Fortran verification utility

### Archived Scripts

Development and debugging scripts have been moved to `scripts/.dustbin/`:

- `rebuild_all_su_with_libpar.bat` - Specific rebuild script
- `rebuild_failed.bat` - Rebuild failed components
- `build_data_compression.bat` - Build specific subset
- `build_all_su_main.bat` - Redundant build script
- `make_all_install.bat` - Redundant install script
- `make_su_main.bat` - Redundant SU main build
- `generate_su_makefiles.bat` - Development tool
- `generate_su_makefiles.ps1` - Development tool
- `update_su_makefiles.ps1` - Development tool
- `update_makefiles_windows.ps1` - Development tool
- `Makefile.su_template` - Template file
- `make_all_libraries.bat` - Consolidated into build.bat
- `build_all_su.bat` - Consolidated into build.bat

These archived scripts are kept for reference but are not part of the standard build workflow.

---

## See Also

- **Windows Build Guide**: `WINDOWS_BUILD_GUIDE.md` - Detailed build instructions
- **User Guide**: `USER_GUIDE.md` - How to use SU programs
- **Changes**: `CHANGES.md` - Port summary and fixes
- **SU Flow GUI**: `zau/src/pysu/gui/README.md` - GUI documentation
- **Python Plugins**: `zau/src/pysu/plugins/README.md` - Plugin system

---

## Quick Reference

| Script | Purpose | Output |
|--------|---------|--------|
| `build.bat` | Build libraries + SU programs | `lib/*.lib`, `bin/*.exe` |
| `clean.bat` | Clean build artifacts | (removes intermediate files) |
| `wipe.bat` | Remove all build outputs | (removes final outputs) |
| `build_suflow.bat` | Build suflow.exe | `bin/suflow.exe` |
| `build_fortran.bat` | Build Fortran components | `bin/*.exe` (Fortran) |
| `build_release.bat` | Build + create distribution | `dist/cwpsu-windows-x64.zip` |
| `verify_fortran.bat` | Check Fortran setup | (status messages) |

---

## Notes

- All scripts automatically detect the project root directory
- Scripts use relative paths and should be run from the `scripts/` directory
- Build scripts set up the Visual Studio environment automatically
- GNU Make is required for all builds
- Fortran components are optional and require Intel oneAPI

