# PySeismicUnix - CWP/SU for Windows

## License

This project includes:
- **CWP/SU** - Original CWP-SU License (Copyright © 2008, Colorado School of Mines)
- **ZAU Enhancements** - MIT License (Copyright © 2025 ZAU.Energy Asia Limited)

See [LICENSE.md](LICENSE.md) for details.

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Executive Summary

PySeismicUnix is a comprehensive Windows port of CWP/SU (Seismic Unix), a powerful suite of seismic data processing tools. This port brings 312+ SU programs to Windows with full binary compatibility, Python integration, and modern workflow tools.

**Key Features:**
- ✅ **312 SU Executables** - Complete seismic processing toolkit
- ✅ **Python Plugin System** - Extend SU with custom Python algorithms
- ✅ **SU Flow GUI** - PyQt6 graphical workflow builder
- ✅ **Windows Binary Pipes** - Reliable pipeline execution via `suflow.exe`
- ✅ **Full Library Support** - 9 core libraries (libcwp, libpar, libsu, etc.)
- ✅ **Fortran Components** - Optional Fortran programs (Cshot, Cwell, Triso, etc.)

**Platform:** Windows 10/11 x64  
**Build System:** Visual Studio 2022 + GNU Make  
**Python:** 3.13.x (for GUI and plugins)

---

## Quick Start

### Installation

1. **Extract distribution** or **build from source** (see [Build Guide](#documentation))
2. **Add `bin\` to PATH**
3. **Test installation:**
   ```cmd
   surange.exe < samples\data.su
   ```

### Basic Usage

```cmd
REM View trace information
surange.exe < input.su

REM Process data
sugain.exe agc=1 < input.su > output.su

REM Use pipelines (Windows-safe)
suflow.exe "sugain agc=1 | sufilter f=10,20,40,50"
```

### Python Integration

```cmd
REM Run Python plugin
pysurun.exe plugin=pysuflipsign < input.su > output.su

REM Launch GUI
cd zau\src\pysu
run_gui.bat
```

---

## Documentation

### User Guides

- **[USER_GUIDE.md](USER_GUIDE.md)** - Complete user guide for SU programs
  - Installation instructions
  - Basic usage and examples
  - Common operations
  - Troubleshooting

- **[PYTHON-GUIDE.md](PYTHON-GUIDE.md)** - Python integration guide
  - Plugin system documentation
  - GUI usage
  - Plugin development
  - Workflow examples

### Build & Development

- **[WINDOWS_BUILD_GUIDE.md](WINDOWS_BUILD_GUIDE.md)** - Building from source
  - Prerequisites and setup
  - Build instructions
  - Compilation details

- **[scripts/README.md](scripts/README.md)** - Build scripts reference
  - `build.bat` - Build libraries and SU programs
  - `clean.bat` - Clean build artifacts
  - `wipe.bat` - Remove all build outputs
  - `build_fortran.bat` - Build Fortran components
  - `build_release.bat` - Create distribution package

- **[include/README.md](include/README.md)** - Windows port header modifications
  - Changes to `cwp.h` and `su_xdr.h`
  - Windows compatibility modifications
  - Implementation details

- **[include/win32_compat/README.md](include/win32_compat/README.md)** - Windows compatibility layer
  - POSIX compatibility implementation
  - File and directory operations
  - Path handling and type system
  - Technical documentation

### Port Information

- **[CHANGES.md](CHANGES.md)** - Port summary and changes
  - What was ported
  - What was deprecated
  - Build statistics
  - Technical details

- **[MAN_PAGES_AND_DISTRIBUTION.md](MAN_PAGES_AND_DISTRIBUTION.md)** - Documentation and distribution
  - Man page generation
  - Distribution creation
  - Package contents

### Component Documentation

- **[zau/src/pysu/plugins/README.md](zau/src/pysu/plugins/README.md)** - Python plugin system
  - Plugin architecture
  - Creating plugins
  - API reference

- **[zau/src/pysu/gui/README.md](zau/src/pysu/gui/README.md)** - SU Flow GUI
  - GUI features
  - Installation and usage
  - Configuration

- **[zau/src/suflow/README.md](zau/src/suflow/README.md)** - suflow.exe binary pipe wrapper
  - Windows pipe solution
  - Usage examples
  - Technical details

- **[zau/src/processtree/README.md](zau/src/processtree/README.md)** - Process tree catalog
  - SU process organization
  - YAML structure

- **[zau/thirdparty/README.md](zau/thirdparty/README.md)** - Third-party packages
  - SCons build system
  - Seismic-Canvas visualization
  - License information

---

## What's Included

### Core Components

- **312 SU Programs** - Complete seismic processing toolkit
  - Amplitude processing, filtering, deconvolution
  - Migration, velocity analysis, stacking
  - Data conversion, header manipulation
  - And much more

- **9 Static Libraries**
  - libcwp, libpar, libsu, libwin32compat
  - libcomp, libtri, libtetra, librefl, libtrielas

- **Windows Enhancements**
  - `suflow.exe` - Binary pipe wrapper
  - `pysurun.exe` - Python plugin bridge
  - SU Flow GUI - PyQt6 workflow builder

### Python Features

- **Plugin System** - Write custom processing in Python
- **GUI Application** - Visual workflow builder
- **Development Tools** - Process tree generation scripts

### Optional Components

- **Fortran Programs** - Cshot, Cwell, Triso, Vplusz, Vzest
  - Requires Intel oneAPI Fortran Compiler

---

## System Requirements

### Essential

- **Windows 10/11** (64-bit)
- **Visual Studio 2022** (for building from source)
- **GNU Make** (for building from source)

### Optional

- **Python 3.13.x** (for GUI and plugins)
- **Intel oneAPI Fortran Compiler** (for Fortran components)

---

## Getting Started

1. **Read [USER_GUIDE.md](USER_GUIDE.md)** for usage instructions
2. **Try sample workflows** in the GUI
3. **Explore Python plugins** - See [PYTHON-GUIDE.md](PYTHON-GUIDE.md)
4. **Build from source** - See [WINDOWS_BUILD_GUIDE.md](WINDOWS_BUILD_GUIDE.md)

---

## Acknowledgments

- **CWP/SU Foundation** - Original Seismic Unix package
- **John W. Stockwell Jr.** - Maintainer of SeisUnix repository
- **SCons Foundation** - Build system (see [zau/thirdparty/README.md](zau/thirdparty/README.md))
- **Yunzhi Shi** - Seismic-Canvas visualization tool (see [zau/thirdparty/README.md](zau/thirdparty/README.md))

---

## Support & Resources

- **User Guide:** [USER_GUIDE.md](USER_GUIDE.md)
- **Python Guide:** [PYTHON-GUIDE.md](PYTHON-GUIDE.md)
- **Build Guide:** [WINDOWS_BUILD_GUIDE.md](WINDOWS_BUILD_GUIDE.md)
- **Changes:** [CHANGES.md](CHANGES.md)

---

*For detailed information, see the documentation links above.*
