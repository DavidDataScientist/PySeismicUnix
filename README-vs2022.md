# Visual Studio 2022 Command Shells Guide

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

This guide explains how to get, install, and use Visual Studio 2022 command shells for building PySeismicUnix (CWP/SU for Windows). It covers both manual building using VS2022 command shells and using the automated build scripts.

---

## Part 1: Getting and Installing Visual Studio 2022

### Download Visual Studio 2022

1. **Visit the official download page:**
   - Go to: https://visualstudio.microsoft.com/downloads/
   - Or search for "Visual Studio 2022 download"

2. **Choose your edition:**
   - **Community Edition** (Free) - Recommended for most users
   - **Professional Edition** (Paid) - For professional development
   - **Enterprise Edition** (Paid) - For enterprise teams

3. **Download the installer:**
   - Click "Download" for your chosen edition
   - The installer is named `vs_community.exe`, `vs_professional.exe`, or `vs_enterprise.exe`

### Install Visual Studio 2022

1. **Run the installer:**
   ```cmd
   REM Run the downloaded installer (e.g., vs_community.exe)
   ```

2. **Select workloads:**
   - **Required:** Check "Desktop development with C++"
   - This includes:
     - MSVC (Microsoft Visual C++ compiler)
     - Windows SDK
     - CMake tools
     - C++ core features

3. **Optional components (recommended):**
   - **Individual components** tab:
     - C++ CMake tools for Windows
     - Windows 10/11 SDK (latest version)
     - MSVC v143 - VS 2022 C++ x64/x86 build tools

4. **Install location:**
   - Default: `C:\Program Files\Microsoft Visual Studio\2022\Community` (or Professional/Enterprise)
   - You can change this, but the scripts expect standard locations

5. **Click "Install"** and wait for installation to complete

### Verify Installation

After installation, verify Visual Studio 2022 is installed:

```cmd
REM Check if vcvarsall.bat exists
dir "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
```

If the file exists, Visual Studio 2022 is installed correctly.

---

## Part 2: Using VS2022 Command Shells for Building

### Finding VS2022 Command Shells

Visual Studio 2022 provides several command prompt shortcuts:

#### Method 1: Start Menu

1. **Open Start Menu**
2. **Search for:** "Developer Command Prompt for VS 2022"
3. **Available options:**
   - **Developer Command Prompt for VS 2022** - x86 (32-bit) tools
   - **x64 Native Tools Command Prompt for VS 2022** - x64 (64-bit) tools ⭐ **Recommended**
   - **x86_x64 Cross Tools Command Prompt for VS 2022** - Cross-compilation tools

#### Method 2: File Explorer

Navigate to:
```
C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat
```
(Replace `Community` with `Professional` or `Enterprise` if applicable)

#### Method 3: Program Files Location

The command shells are typically located at:
- **Community:** `C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\`
- **Professional:** `C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\`
- **Enterprise:** `C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\`

### Opening a VS2022 Command Shell

#### Option A: Use Start Menu Shortcut (Easiest)

1. Click **Start Menu**
2. Type: **"x64 Native Tools Command Prompt for VS 2022"**
3. Click the shortcut
4. A command prompt window opens with the VS2022 environment already configured

#### Option B: Manual Setup in Regular Command Prompt

If you're already in a regular command prompt, you can initialize the VS2022 environment:

```cmd
REM For Community Edition (x64)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

REM For Professional Edition (x64)
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64

REM For Enterprise Edition (x64)
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
```

#### Option C: Use PowerShell

```powershell
# For Community Edition (x64)
& "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

# For Professional Edition (x64)
& "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64

# For Enterprise Edition (x64)
& "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
```

### Verifying the VS2022 Environment

After opening a VS2022 command shell, verify the environment is set up correctly:

```cmd
REM Check if cl.exe (MSVC compiler) is available
cl

REM Check if link.exe (linker) is available
link

REM Check compiler version
cl /?

REM Check environment variables
echo %VCINSTALLDIR%
echo %INCLUDE%
echo %LIB%
```

You should see:
- `cl.exe` command recognized
- `link.exe` command recognized
- Environment variables set (VCINSTALLDIR, INCLUDE, LIB, PATH)

### Building Manually (Without Scripts)

If you want to build manually without using the provided scripts, follow these steps:

#### Step 1: Set Up Environment

```cmd
REM Open x64 Native Tools Command Prompt for VS 2022
REM Or manually initialize:
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

REM Add GNU Make to PATH (if not already there)
set PATH=C:\Tools\gnu\bin;%PATH%

REM Set CWPROOT environment variable
set CWPROOT=D:\src\proto\PySeismicUnix
cd /d %CWPROOT%
```

#### Step 2: Build Libraries

Build libraries in order (they have dependencies):

```cmd
REM Build libcwp
cd %CWPROOT%\src\cwp\lib
make

REM Build libpar
cd %CWPROOT%\src\par\lib
make

REM Build libsu
cd %CWPROOT%\src\su\lib
make

REM Build libwin32compat
cd %CWPROOT%\src\win32_compat
make
```

#### Step 3: Build SU Programs

```cmd
REM Navigate to a specific SU program directory
cd %CWPROOT%\src\su\main\amplitudes

REM Build the program
make -f Makefile INSTALL

REM Repeat for other directories:
REM - attributes_parameter_estimation
REM - convolution_correlation
REM - data_compression
REM - datuming
REM - decon_shaping
REM - dip_moveout
REM - filters
REM - interp_extrap
REM - migration_inversion
REM - multicomponent
REM - noise
REM - operations
REM - picking
REM - stacking
REM - statics
REM - stretching_moveout_resamp
REM - supromax
REM - synthetics_waveforms_testpatterns
REM - tapering
REM - transforms
REM - velocity_analysis
REM - well_logs
REM - windowing_sorting_muting
```

#### Step 4: Verify Build

```cmd
REM Check libraries were built
dir %CWPROOT%\lib\*.lib

REM Check executables were built
dir %CWPROOT%\bin\*.exe

REM Test a program
%CWPROOT%\bin\surange.exe
```

---

## Part 3: Using the Build Scripts

The project includes automated build scripts that handle all the environment setup and build process for you. **This is the recommended approach.**

### Prerequisites for Scripts

1. **Visual Studio 2022** installed (see Part 1)
2. **GNU Make** installed at `C:\Tools\gnu\bin\make.exe`
   - Download from: https://gnuwin32.sourceforge.net/packages/make.htm
   - Or use MSYS2/MinGW make
3. **Python 3.13.x** (optional, for GUI and plugins)

### Scripts Overview

All build scripts are located in the `scripts\` directory:

| Script | Purpose |
|--------|---------|
| `build.bat` | Build all libraries and SU programs (no Fortran) |
| `build_fortran.bat` | Build Fortran components (requires Intel oneAPI) |
| `build_release.bat` | Build everything and create distribution package |
| `clean.bat` | Clean intermediate build artifacts |
| `wipe.bat` | Remove all final build outputs |
| `verify_fortran.bat` | Check Fortran compiler setup |

### Using the Scripts

#### Option A: From Regular Command Prompt

The scripts automatically detect and initialize Visual Studio 2022, so you can run them from a regular command prompt:

```cmd
REM Navigate to scripts directory
cd D:\src\proto\PySeismicUnix\scripts

REM Build all libraries and SU programs
build.bat

REM Build Fortran components (if needed)
build_fortran.bat

REM Create distribution package
build_release.bat
```

#### Option B: From VS2022 Command Shell

You can also run the scripts from a VS2022 command shell (though it's not required):

```cmd
REM Open x64 Native Tools Command Prompt for VS 2022
REM Navigate to scripts directory
cd D:\src\proto\PySeismicUnix\scripts

REM Run build script
build.bat
```

### Build Workflows

#### Standard Build (C/C++ Only)

```cmd
cd scripts
build.bat
```

This builds:
- All libraries (libcwp, libpar, libsu, libwin32compat)
- All SU programs (312+ executables)

#### Clean Build

```cmd
cd scripts
clean.bat
build.bat
```

Cleans intermediate artifacts, then rebuilds from scratch.

#### Complete Wipe and Rebuild

```cmd
cd scripts
wipe.bat
clean.bat
build.bat
```

Removes all final outputs, cleans intermediate artifacts, then rebuilds from scratch.

#### Full Build (Including Fortran)

```cmd
cd scripts
build.bat
build_fortran.bat
```

Builds all C/C++ components, then builds Fortran components (requires Intel oneAPI).

#### Release Build

```cmd
cd scripts
build_release.bat
```

Builds everything and creates a distribution package in `dist\`.

### What the Scripts Do Automatically

The build scripts handle:

1. **VS2022 Detection:**
   - Automatically finds Visual Studio 2022 in standard locations
   - Supports Community, Professional, and Enterprise editions
   - Checks both `Program Files` and `Program Files (x86)`

2. **Environment Setup:**
   - Calls `vcvarsall.bat` to initialize MSVC environment
   - Sets up compiler, linker, and library paths
   - Adds GNU Make to PATH

3. **Build Process:**
   - Builds libraries in correct dependency order
   - Builds all SU programs systematically
   - Provides progress feedback and error reporting

4. **Error Handling:**
   - Reports which components failed to build
   - Provides summary statistics
   - Lists built libraries and executables

### Script Output

The scripts provide detailed output:

```
========================================
Building Libraries and SU Programs
========================================

CWPROOT = D:\src\proto\PySeismicUnix

========================================
Building Libraries
========================================

Building CWP library (libcwp)...
----------------------------------------
[PASS] libcwp

Building PAR library (libpar)...
----------------------------------------
[PASS] libpar

...

========================================
Build Summary
========================================

Libraries:
  Successful: 4
  Failed: 0

SU Programs:
  Successful: 312
  Failed: 0

Libraries in lib directory:
libcwp.lib
libpar.lib
libsu.lib
libwin32compat.lib

Total executables in bin: 312
```

### Troubleshooting Script Issues

#### "Visual Studio 2022 not found"

**Solution:**
- Ensure Visual Studio 2022 is installed
- Check that `vcvarsall.bat` exists in the expected location
- Try running from Visual Studio Developer Command Prompt manually

#### "GNU Make not found"

**Solution:**
- Install GNU Make
- Ensure it's at `C:\Tools\gnu\bin\make.exe`
- Or add it to your PATH

#### "Build fails with linker errors"

**Solution:**
- Ensure all libraries built successfully first
- Try a clean build: `clean.bat` then `build.bat`
- For a complete fresh start: `wipe.bat` then `clean.bat` then `build.bat`

#### "Some programs fail to build"

**Solution:**
- Check error messages in the build output
- Ensure all dependencies (libraries) built successfully
- Verify CWPROOT is set correctly
- Try building individual components manually to isolate the issue

---

## Comparison: Manual vs. Scripts

### Manual Building (VS2022 Command Shell)

**Pros:**
- Full control over build process
- Can build individual components
- Good for debugging specific issues
- Educational - understand the build process

**Cons:**
- More steps to remember
- Must manually set up environment each time
- Easy to miss dependencies
- More error-prone

### Using Build Scripts

**Pros:**
- Automated environment setup
- Handles dependencies automatically
- Consistent build process
- Error reporting and summaries
- Less prone to mistakes

**Cons:**
- Less granular control
- May rebuild more than necessary

**Recommendation:** Use the build scripts for regular builds. Use manual building for debugging or when you need to build specific components.

---

## Quick Reference

### VS2022 Command Shell Locations

| Edition | Path |
|---------|------|
| Community | `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat` |
| Professional | `C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat` |
| Enterprise | `C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat` |

### Common Commands

```cmd
REM Initialize VS2022 environment (x64)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

REM Verify compiler
cl

REM Build with scripts
cd scripts
build.bat

REM Clean build
cd scripts
clean.bat
build.bat
```

### Environment Variables Set by vcvarsall.bat

- `VCINSTALLDIR` - Visual Studio installation directory
- `INCLUDE` - Header file search paths
- `LIB` - Library file search paths
- `PATH` - Updated with compiler and tool paths
- `VSCMD_ARG_TGT_ARCH` - Target architecture (x64, x86, etc.)

---

## Additional Resources

- **[scripts/README.md](scripts/README.md)** - Detailed build scripts reference
- **[WINDOWS_BUILD_GUIDE.md](WINDOWS_BUILD_GUIDE.md)** - Complete Windows build guide
- **[Visual Studio 2022 Documentation](https://docs.microsoft.com/en-us/visualstudio/)** - Official VS2022 docs

---

*Document created: January 2025*  
*For PySeismicUnix (CWP/SU for Windows)*

