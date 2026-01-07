# Fortran Components Guide

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

This project includes **some** Fortran components that are built using **Intel oneAPI Fortran Compiler**. These Fortran tools are optional and are not required for the core CWP/SU functionality.

**Important:** This guide provides basic information and examples, but **you are largely on your own** when it comes to building, modifying, or troubleshooting Fortran components beyond what's documented here.

---

## What Fortran Components Are Included?

The following Fortran components are available in `src/Fortran/`:

| Component | Description | Status |
|-----------|-------------|--------|
| **Cshot** | Shot gather processing and modeling | ✅ Built by `build_fortran.bat` |
| **Cwell** | Well log processing | ✅ Built by `build_fortran.bat` |
| **Triso** | 3D transversely isotropic processing | ✅ Built by `build_fortran.bat` |
| **Vplusz** | Velocity analysis | ✅ Built by `build_fortran.bat` |
| **Vzest** | Velocity estimation | ✅ Built by `build_fortran.bat` |
| **AzimVelan** | Azimuthal velocity analysis | ⚠️ Not built by default |
| **Cxzco** | 2.5D common offset inversion | ⚠️ Not built by default |
| **Cxzcs** | 2.5D common shot inversion | ⚠️ Not built by default |
| **Raytrace3D** | 3D ray tracing | ⚠️ Not built by default |

**Note:** Only the first 5 components (Cshot, Cwell, Triso, Vplusz, Vzest) are built by the automated `build_fortran.bat` script. The others require manual building if you need them.

---

## Prerequisites

### Required Software

1. **Visual Studio 2022**
   - Community, Professional, or Enterprise edition
   - C++ development tools
   - Required for MSVC runtime libraries

2. **Intel oneAPI Base Toolkit**
   - **Download:** https://www.intel.com/content/www/us/en/developer/tools/oneapi/toolkits.html
   - **Required component:** Intel Fortran Compiler
   - **Recommended compiler:** `ifx.exe` (newer, LLVM-based)
   - **Legacy compiler:** `ifort.exe` (older, still supported)

3. **GNU Make**
   - Expected location: `C:\Tools\gnu\bin\make.exe`
   - Required for building with Makefiles

### Installation Locations

The build scripts look for Intel oneAPI in these locations:
- `C:\Tools\Intel\oneAPI\`
- `C:\Program Files (x86)\Intel\oneAPI\`

If you install elsewhere, you'll need to modify the build scripts or set up your environment manually.

---

## Quick Start

### Step 1: Verify Fortran Compiler

```cmd
cd scripts
verify_fortran.bat
```

This checks for:
- Intel `ifx.exe` (recommended)
- Intel `ifort.exe` (legacy)
- Intel oneAPI environment setup

### Step 2: Build Fortran Components

```cmd
cd scripts
build_fortran.bat
```

This builds:
- Cshot (cshot1.exe, cshot2.exe, cshotplot.exe)
- Cwell (cwell.exe)
- Triso (triso.exe, triview.exe, trisoplot.exe)
- Vplusz (vplusz.exe)
- Vzest (vzest.exe)

### Step 3: Verify Build

```cmd
REM Check executables were built
dir bin\cshot*.exe
dir bin\cwell.exe
dir bin\triso*.exe
dir bin\vplusz.exe
dir bin\vzest.exe
```

---

## Examples Provided

Each Fortran component includes example directories with demo scripts and data:

### Cshot Examples

Location: `src/Fortran/Cshot/Demo01/` through `Demo14/`

Each demo directory contains:
- Parameter files (`param1`, `param2`, etc.)
- Geometry files (`geometry1`, `geometry2`, etc.)
- Model files (`simplemodel`, `syncline`, etc.)
- Example run scripts (`Xcshot`, `PScshot`)
- README or NOTES files

**To use:**
1. Copy a demo directory to your work area
2. Read the NOTES or README file
3. Run the example scripts (may need Windows adaptation)

### Cwell Examples

Location: `src/Fortran/Cwell/Demo01/` through `Demo11/`

Similar structure to Cshot with:
- Parameter files
- Well log data
- Example scripts (`Xcwell`, `PScwell`)

### Triso Examples

Location: `src/Fortran/Triso/Demo/`

Contains:
- Input data files
- Example parameter files
- Usage instructions

### Other Components

- **Vplusz** and **Vzest** have README files with basic usage
- **Cxzco** and **Cxzcs** have Demo directories with inversion examples
- **AzimVelan** has a Demo directory with azimuthal velocity analysis examples
- **Raytrace3D** has a Demo directory with 3D ray tracing examples

---

## Manual Building (If Scripts Don't Work)

If the automated build scripts don't work for your setup, you can build manually:

### Step 1: Set Up Environment

```cmd
REM Initialize Visual Studio 2022
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

REM Initialize Intel oneAPI
call "C:\Tools\Intel\oneAPI\setvars.bat"
REM OR
call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat"

REM Add GNU Make to PATH
set PATH=C:\Tools\gnu\bin;%PATH%

REM Set CWPROOT
set CWPROOT=D:\src\proto\PySeismicUnix
cd /d %CWPROOT%
```

### Step 2: Build a Component

```cmd
REM Example: Build Cshot
cd src\Fortran\Cshot
make CWPROOT=%CWPROOT% INSTALL
```

### Step 3: Verify

```cmd
REM Check if executables were created
dir ..\..\..\bin\cshot*.exe
```

---

## Hints and Suggestions

### Compiler Selection

- **Use `ifx.exe` if available** - It's the newer LLVM-based compiler with better optimization
- **`ifort.exe` works too** - Legacy compiler, still fully supported
- **Avoid gfortran** - While it may work, the Makefiles are designed for Intel Fortran

### Environment Setup

- **Always initialize VS2022 first** - Fortran components link against MSVC runtime libraries
- **Then initialize Intel oneAPI** - This sets up Fortran compiler paths
- **Check your PATH** - Both compilers need to be in PATH for `make` to find them

### Common Issues

#### "ifx.exe not found"

**Solution:**
- Ensure Intel oneAPI is installed
- Run `setvars.bat` to initialize environment
- Check PATH includes: `C:\Tools\Intel\oneAPI\compiler\latest\bin` (or Program Files equivalent)

#### "Linker errors about missing symbols"

**Solution:**
- Ensure all C libraries built first: `build.bat` before `build_fortran.bat`
- Check that `libcwp.lib`, `libpar.lib` exist in `lib/` directory
- Verify MSVC runtime libraries are available (vcruntime140.lib, ucrt.lib)

#### "Cannot find ifconsol.lib"

**Solution:**
- The build script should copy this automatically
- If not, manually copy from Intel oneAPI lib directory to `lib/`:
  ```cmd
  copy "C:\Tools\Intel\oneAPI\compiler\latest\lib\intel64\ifconsol.lib" lib\
  ```

#### "Build succeeds but executable doesn't run"

**Solution:**
- Check for missing DLLs: `libifcoremd.dll`, `libmmd.dll`
- These should be in Intel oneAPI bin directory
- Add Intel oneAPI bin to PATH or copy DLLs to `bin/` directory

### Building Additional Components

To build components not included in `build_fortran.bat`:

```cmd
REM Example: Build AzimVelan
cd src\Fortran\AzimVelan
make CWPROOT=%CWPROOT% INSTALL

REM Example: Build Cxzco
cd src\Fortran\Cxzco
make CWPROOT=%CWPROOT% INSTALL
```

**Warning:** These components may have additional dependencies or requirements not documented here.

### Modifying Fortran Code

If you need to modify Fortran source code:

1. **Understand the Makefile structure:**
   - Fortran sources (`.f` files) compile to `.obj` files
   - Objects are archived into `.lib` files
   - Programs link against the library and C libraries

2. **Compiler flags are in `Makefile.config`:**
   - Located at `src/Makefile.config`
   - `FFLAGS` controls Fortran compilation
   - `LFLAGS` controls linking

3. **Windows-specific considerations:**
   - Use `/MD` for DLL runtime (multithreaded)
   - Link against `ifconsol.lib` for console programs
   - May need `/SUBSYSTEM:CONSOLE` for linker

### Debugging

- **Use `/Zi` flag** for debug symbols (already in Makefiles)
- **Check `.pdb` files** - Debug info files should be generated
- **Use Intel Fortran debugger** - If available in your oneAPI installation
- **Check compiler output** - Intel compilers provide detailed error messages

### Performance Optimization

- **Default optimization level** is usually `-O2` or `/O2`
- **For maximum performance**, try `/O3` (but test thoroughly)
- **Profile-guided optimization** may be available in newer Intel compilers
- **Consider parallelization** - Some Fortran code may benefit from OpenMP

---

## Makefile Structure

Each Fortran component has a `Makefile` that:

1. **Includes common configuration:**
   ```makefile
   include $(CWPROOT)/src/Makefile.config
   ```

2. **Defines source files:**
   ```makefile
   FSRCS = contin.f shoot.f raydat.f ...
   ```

3. **Compiles to objects:**
   ```makefile
   %.obj: %.f
       $(FC) $(FFLAGS) /Fo$@ $<
   ```

4. **Creates library:**
   ```makefile
   $(LIB): $(FOBJS)
       $(AR) $(ARFLAGS)$@ $(FOBJS)
   ```

5. **Links programs:**
   ```makefile
   $(PROGS): $(FTARGET) $(LIB)
       $(FC) ... /link ... /OUT:"$@"
   ```

**Key variables:**
- `FC` - Fortran compiler (ifx or ifort)
- `FFLAGS` - Fortran compiler flags
- `LFLAGS` - Linker flags
- `AR` - Archive tool (lib.exe on Windows)
- `CWPROOT` - Project root directory

---

## Documentation References

### Component-Specific READMEs

Each component has its own README or documentation:
- `src/Fortran/Cshot/README` - Cshot documentation
- `src/Fortran/Cwell/README` - Cwell documentation
- `src/Fortran/Triso/README` - Triso documentation
- `src/Fortran/Vplusz/README` - Vplusz usage
- `src/Fortran/Vzest/README` - Vzest usage

### Original CWP Documentation

Many components reference original CWP documentation:
- CWP Release Notes (PDFs in component directories)
- Original CWP/SU documentation
- Component-specific papers and references

### Intel oneAPI Documentation

- **Intel Fortran Compiler Documentation:** https://www.intel.com/content/www/us/en/developer/tools/oneapi/fortran-compiler.html
- **Intel oneAPI Base Toolkit:** https://www.intel.com/content/www/us/en/developer/tools/oneapi/toolkits.html

---

## Limitations and Warnings

### What We Provide

✅ **Build scripts** for 5 main components (Cshot, Cwell, Triso, Vplusz, Vzest)  
✅ **Example directories** with demo scripts and data  
✅ **Basic documentation** in component READMEs  
✅ **Working Makefiles** for Windows builds  

### What We Don't Provide

❌ **Detailed usage tutorials** for each component  
❌ **Support for modifying or extending** Fortran code  
❌ **Troubleshooting guides** for component-specific issues  
❌ **Guarantees** that all components will build or run correctly  
❌ **Support for alternative compilers** (gfortran, etc.)  

### You're On Your Own For

- Understanding what each component does
- Learning how to use the components
- Adapting Unix shell scripts to Windows
- Fixing build issues beyond basic setup
- Modifying or extending Fortran code
- Integrating with other tools
- Performance tuning and optimization

---

## Getting Help

### If Build Scripts Fail

1. **Check prerequisites** - VS2022, Intel oneAPI, GNU Make
2. **Run `verify_fortran.bat`** - Ensure compiler is found
3. **Check error messages** - They often indicate what's missing
4. **Try manual build** - See "Manual Building" section above
5. **Check component README** - May have component-specific requirements

### If Components Don't Run

1. **Check dependencies** - Ensure C libraries built first
2. **Check DLLs** - Intel Fortran runtime DLLs must be available
3. **Check PATH** - May need Intel oneAPI bin in PATH
4. **Read component README** - May have usage requirements

### If You Need More Help

- **Component READMEs** - Start here for component-specific info
- **Example directories** - Study the demo scripts
- **Intel oneAPI documentation** - For compiler-specific issues
- **Original CWP documentation** - For algorithm and usage details

**Note:** This project does not provide active support for Fortran components beyond what's documented here.

---

## Summary

- ✅ **5 components** are built automatically by `build_fortran.bat`
- ✅ **Example directories** are provided for learning
- ✅ **Basic build infrastructure** is in place
- ⚠️ **You're largely on your own** for usage, modification, and troubleshooting
- ⚠️ **Additional components** require manual building
- ⚠️ **No guarantees** about functionality or compatibility

**Recommendation:** Start with the examples provided, read the component READMEs, and be prepared to do your own research and experimentation.

---

## Quick Reference

```cmd
REM Verify Fortran setup
cd scripts
verify_fortran.bat

REM Build all supported Fortran components
cd scripts
build_fortran.bat

REM Build a specific component manually
cd src\Fortran\Cshot
make CWPROOT=%CD%\..\..\.. INSTALL

REM Check what was built
dir ..\..\..\bin\*.exe
```

---

*Document created: January 2025*  
*For PySeismicUnix (CWP/SU for Windows)*

