MIT License

Copyright (c) 2025 ZAU.Energy Asia Limited

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---

## Scope of This License

This license applies to the following ZAU additions and enhancements to the
CWP/SU (Seismic Unix) codebase:

### Windows Porting Components

- **Windows Build System**: Makefile conversions, build scripts, and build
  automation tools in the `scripts/` directory
- **Windows Compatibility Layer**: `libwin32compat.lib` and related compatibility
  code for Windows-specific functionality
- **Windows Build Configuration**: Makefile.config and related build
  configuration files adapted for Visual Studio 2022 and MSVC

### Custom Tools and Utilities

- **suflow.exe**: Windows binary pipe wrapper (`zau/src/suflow/suflow.c`)
  - Solves Windows pipe corruption issues for SU pipelines
  - Enables reliable binary data transfer between SU programs

- **pysurun.exe**: Python plugin bridge (`src/su/main/operations/pysurun.c`)
  - Integrates Python plugins into SU command pipelines
  - Provides interface between SU programs and Python processing

### Python Integration

- **SU Flow GUI**: PyQt6-based graphical workflow builder
  - Location: `zau/src/pysu/gui/suflow_gui.py`
  - Interactive GUI for building and executing SU workflows

- **Python Plugin System**: Plugin architecture and framework
  - Location: `zau/src/pysu/plugins/`
  - Base classes: `SuFlowPlugin`, `SuTrace`
  - Factory pattern for plugin registration
  - Example plugins (e.g., `pysuflipsign.py`)

- **Python Development Tools**: Process tree generation scripts
  - Location: `zau/scripts/`
  - `generate_process_tree_yaml.py`
  - `create_yaml_direct.py`

### Documentation

- **User Documentation**: All ZAU-authored documentation files
  - `USER_GUIDE.md` - Windows user guide
  - `PYTHON-GUIDE.md` - Python integration guide
  - `WINDOWS_BUILD_GUIDE.md` - Build instructions
  - `CHANGES.md` - Port summary and changes
  - `README.md` - Executive summary
  - Component-specific README files in `zau/` subdirectories

### Build and Distribution Scripts

- **Build Scripts**: Located in `scripts/`
  - `build.bat` - Main build script
  - `clean.bat` - Clean build artifacts
  - `wipe.bat` - Remove build outputs
  - `build_fortran.bat` - Fortran component builder
  - `build_release.bat` - Distribution creator
  - `verify_fortran.bat` - Fortran compiler verification

### Process Tree and Catalog

- **Process Tree System**: YAML-based process catalog
  - Location: `zau/src/processtree/`
  - `su_process_tree.yaml` - Process organization catalog
  - Generation scripts and tools

### Third-Party Package Modifications

- **Modified Third-Party Packages**: Located in `zau/thirdparty/`
  - SCons build system modifications
  - Seismic-Canvas modifications
  - Note: Original copyrights and licenses of third-party packages remain
    with their respective owners. See `zau/thirdparty/README.md` for details.

---

## Relationship to CWP/SU License

This license applies **only** to the ZAU additions and enhancements listed
above. The original CWP/SU codebase and fixes to that codebase remain under 
the Colorado School of Mines license as described in `LICENSE.md`.

When distributing this software:
- The CWP/SU components must retain their original copyright notices and
  license terms
- The ZAU components are licensed under this MIT License
- **Both licenses must be included in distributions**

---

## Attribution

When using or distributing ZAU enhancements, please include attribution:

```
PySeismicUnix - Windows Port
Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License
Author - David Markus <david@zau.ai>
```

For publications or documentation, you may reference:

```
David Markus, PySeismicUnix: CWP/SU Windows Port with Python Integration, 
ZAU.Energy Asia Limited. (2025) <https://github.com/DavidDataScientist/PySeismicUnix>
```

---

## Contact

For questions regarding this license or ZAU contributions, please refer to the
project documentation or contact ZAU.Energy Asia Limited.

