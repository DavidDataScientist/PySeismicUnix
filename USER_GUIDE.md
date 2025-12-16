# CWP/SU Windows User Guide

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Introduction

This is a Windows port of CWP/SU (Seismic Unix), a comprehensive suite of seismic data processing tools. This guide explains how to use the Windows version effectively.

**Version:** Windows x64  
**Build Date:** December 2024  
**Total Programs:** 312 executables

---

## Installation

### Quick Install

1. Extract `cwpsu-windows-x64.zip` to your desired location
2. Add the `bin\` directory to your system PATH:
   ```batch
   set PATH=%PATH%;D:\path\to\cwpsu-windows-x64\bin
   ```
3. Test installation:
   ```batch
   surange.exe < samples\data.su
   ```

### GUI Installation (Optional)

The PyQt6 GUI requires Python 3.x:

```batch
cd gui
setup_gui.bat    REM Creates virtual environment, installs PyQt6
run_gui.bat      REM Launches the GUI
```

---

## Basic Usage

### File I/O (Recommended)

SU programs read from stdin and write to stdout:

```batch
REM View trace range
surange.exe < input.su

REM Apply gain and save
sugain.exe agc=1 < input.su > output.su

REM Multiple operations
sufilter.exe f=10,20,40,50 < input.su > filtered.su
```

### Pipelines

**Important:** Windows cmd/PowerShell pipes corrupt binary SU data. Use `suflow.exe`:

```batch
REM Simple pipeline
suflow.exe "suplane | sugain agc=1 | sufilter f=10,20,40,50"

REM With input file
suflow.exe "surange < input.su | sugain agc=1 | sufilter f=10,20,40,50"

REM Complex workflow
suflow.exe "suplane | sugain agc=1 | sufilter f=10,20,40,50 | suspecfx | suximage"
```

### Python Plugins

**`pysurun.exe`** allows you to run Python-based plugins in SU pipelines:

```batch
REM Basic usage with Python plugin
pysurun.exe plugin=pysuflipsign < input.su > output.su

REM In a pipeline
suflow.exe "sugain agc=1 | pysurun plugin=pysuflipsign | surange"

REM With virtual environment
pysurun.exe plugin=pysuflipsign venv=./venv < input.su > output.su
```

**Parameters:**
- **`plugin=name`** (required) - Name of Python plugin to execute
- **`python=python`** (optional) - Python interpreter command (default: `python`)
- **`venv=path`** (optional) - Path to virtual environment
- **`module=zau.src.pysu.plugins`** (optional) - Python module path
- **`verbose=0`** (optional) - Set to `1` for verbose output

**Available Plugins:**
- `pysuflipsign` - Flips the sign of all trace samples

**Note:** Python plugins must be installed in your Python environment. See `zau/src/pysu/plugins/README.md` for creating custom plugins.

### GUI Workflows

Launch the SU Flow GUI:
```batch
cd gui
run_gui.bat
```

The GUI provides:
- Interactive console output
- Flow editor with syntax highlighting
- 6 preset test workflows
- File browser for input data

---

## Common Operations

### Viewing Data

```batch
REM Trace header information
surange.exe < data.su
sugethw.exe < data.su key=tracl,tracr,offset

REM Display trace values
supswiggle.exe < data.su > output.ps
```

### Filtering

```batch
REM Bandpass filter
sufilter.exe f=10,20,40,50 < input.su > output.su

REM High-pass filter
sufilter.exe f=20,30 < input.su > output.su

REM Low-pass filter
sufilter.exe f=0,0,40,50 < input.su > output.su
```

### Gain Control

```batch
REM Automatic gain control
sugain.exe agc=1 < input.su > output.su

REM Exponential gain
sugain.exe tpow=2.0 < input.su > output.su

REM Balance traces
sugain.exe balance=1 < input.su > output.su
```

### Data Conversion

```batch
REM Convert SEGY to SU
segyread.exe tape=input.segy > output.su

REM Convert SU to SEGY
segywrite.exe < input.su tape=output.segy

REM Extract headers
suheadermath.exe < input.su > headers.txt
```

### Trace Operations

```batch
REM Stack traces
sustack.exe < input.su > output.su

REM Mute traces
sumute.exe t0=0.5 < input.su > output.su

REM Sort traces
susort.exe cdp offset < input.su > output.su
```

---

## Program Categories

### Data I/O
- `surange` - Display trace range information
- `sugethw` - Extract header words
- `segyread` - Read SEGY format
- `segywrite` - Write SEGY format
- `bison2su` - Convert BISON-2 to SU

### Amplitude Processing
- `sugain` - Apply gain
- `suamp` - Amplitude operations
- `sutrcount` - Count traces

### Filtering
- `sufilter` - Bandpass filter
- `sufrac` - Fractional derivative
- `suk1k2filter` - 2D filter

### Deconvolution
- `sudecon` - Deconvolution
- `sushape` - Shaping filter
- `supef` - Predictive error filter

### Migration
- `sumig` - Migration
- `sumigfd` - Finite difference migration
- `sumigps` - Phase shift migration

### Velocity Analysis
- `suvspec` - Velocity spectrum
- `sustolt` - Stolt migration
- `sunmo` - Normal moveout

### Specialized Tools
- `tetramod` - Tetrahedral modeling
- `trimodel` - Triangle modeling
- `sureflpsvsh` - Reflection coefficients
- `sudctcomp` - DCT compression

### Python Integration
- `pysurun` - Run Python plugins in SU pipelines
- `suflow` - Windows binary pipe wrapper for pipelines

---

## Tips and Best Practices

### 1. Always Use File Redirection for Binary Data

✅ **Good:**
```batch
sugain.exe agc=1 < input.su > output.su
```

❌ **Bad (corrupts data):**
```batch
sugain.exe agc=1 < input.su | sufilter.exe > output.su
```

✅ **Use suflow.exe for pipelines:**
```batch
suflow.exe "sugain agc=1 < input.su | sufilter f=10,20,40,50 > output.su"
```

### 2. Check Data Before Processing

```batch
REM Always check trace range first
surange.exe < input.su

REM Verify header information
sugethw.exe < input.su key=ns,dt,tracl
```

### 3. Use Absolute Paths When Needed

```batch
REM If working in different directories
surange.exe < D:\data\input.su
```

### 4. Test with Sample Data

```batch
REM Generate test data
suplane.exe > test.su

REM Process it
sugain.exe agc=1 < test.su > test_gain.su

REM Verify
surange.exe < test_gain.su
```

---

## Troubleshooting

### "Program not found"

Add `bin\` to your PATH or use full path:
```batch
D:\path\to\cwpsu-windows-x64\bin\surange.exe < data.su
```

### "Cannot open include file"

This error should not occur when running executables. If building from source, see `doc\WINDOWS_BUILD_GUIDE.md`.

### Output file is corrupted

- Use file redirection (`<` and `>`) instead of pipes
- Use `suflow.exe` for pipelines
- Check that input file is valid SU format

### GUI won't start

1. Ensure Python 3.x is installed
2. Run `setup_gui.bat` to create virtual environment
3. Check that PyQt6 installed correctly

---

## Getting Help

### Program Documentation

Most SU programs have built-in help:
```batch
surange.exe
REM Shows usage and parameters
```

### Online Resources

- **CWP/SU Homepage:** https://www.cwp.mines.edu/cwpcodes/
- **Seismic Unix Wiki:** https://wiki.seismic-unix.org/
- **Documentation:** See `doc\` directory

### Python Plugin Documentation

For detailed information on creating and using Python plugins:
- **Plugin System:** `zau/src/pysu/plugins/README.md`
- **Plugin API:** See plugin base classes and examples

### Example Workflows

See the GUI's preset workflows for common processing sequences:
1. Basic filtering
2. Gain + filter
3. Deconvolution
4. Migration prep
5. Velocity analysis
6. Full processing flow

---

## Limitations on Windows

### Not Available

- **Graphics:** PostScript plotting, X11 windows, Motif GUIs
- **SEG-D Tape Reading:** Requires SFIO library (Unix-only)
- **Some Third-Party Tools:** Tools requiring gdbm, netinet headers

### Workarounds

- **Graphics:** Use Python matplotlib or export to SEGY for visualization
- **SEG-D:** Convert on Unix system, transfer as SU/SEGY
- **Pipelines:** Use `suflow.exe` instead of shell pipes

---

## Next Steps

1. **Explore Sample Data:** Process files in `samples\`
2. **Try the GUI:** Launch `gui\run_gui.bat`
3. **Try Python Plugins:** Use `pysurun` to extend SU with Python
4. **Read Documentation:** Check `doc\WINDOWS_BUILD_GUIDE.md` for advanced topics
5. **Review Changes:** See `doc\CHANGES.md` for port details

---

*Happy processing!*

