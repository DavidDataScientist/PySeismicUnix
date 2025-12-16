# SU Flow - Windows Binary Pipe Wrapper

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

`suflow.exe` is a Windows-specific utility that enables reliable binary data transfer between SU (Seismic Unix) programs in pipeline workflows. It solves the critical problem of Windows pipes corrupting binary seismic data due to automatic CR/LF translation.

## Problem Statement

On Windows, standard pipes (`|`) operate in text mode by default, which causes:
- Automatic conversion of line endings (CR/LF ↔ LF)
- Corruption of binary SU trace data
- Invalid seismic processing results

Direct file redirection (`<` and `>`) works correctly, but piping between programs fails.

## Solution

`suflow.exe` provides a wrapper that:
- Parses pipeline strings (commands separated by `|`)
- Executes commands sequentially using temporary files for intermediate data
- Ensures binary-mode data transfer without corruption
- Automatically cleans up temporary files
- Supports stdin/stdout or explicit file I/O

## Usage

### Basic Syntax

```cmd
suflow "cmd1 | cmd2 | cmd3" [input.su] [output.su]
```

### Examples

**Using stdin/stdout:**
```cmd
suflow "bin\sugain.exe agc=1 | bin\sufilter.exe" < input.su > output.su
```

**Using explicit files:**
```cmd
suflow "bin\sugain.exe agc=1 | bin\sufilter.exe" input.su output.su
```

**Multi-stage pipeline:**
```cmd
suflow "bin\sugain.exe agc=1 | bin\sufilter.exe | bin\sustack.exe" data.su result.su
```

**Complex workflow:**
```cmd
suflow "bin\sugain.exe tpow=2 | bin\sunormalize.exe | bin\sugain.exe scale=0.5 | bin\surange.exe" input.su
```

## How It Works

1. **Pipeline Parsing**: The pipeline string is parsed into individual commands, respecting quoted arguments.

2. **Temporary Files**: For pipelines with multiple commands, temporary files are created in the system temp directory:
   - Format: `suflow_tmp_<PID>_<index>.su`
   - Automatically deleted after execution

3. **Command Execution**: Each command is executed sequentially:
   - First command: reads from input file (or stdin) → writes to temp file
   - Intermediate commands: read from previous temp file → write to next temp file
   - Last command: reads from temp file → writes to output file (or stdout)

4. **Binary Mode**: All file operations use binary mode to prevent data corruption.

## Building

### Prerequisites

- **Visual Studio 2022** (or compatible MSVC compiler)
- `cl.exe` must be in PATH (use Developer Command Prompt)

### Build Methods

#### Method 1: Using build.bat

```cmd
cd zau\src\suflow
build.bat
```

This script:
- Checks for `cl.exe` availability
- Compiles `suflow.c` with optimized settings
- Installs `suflow.exe` to `bin\` directory

#### Method 2: Using Makefile

```cmd
cd zau\src\suflow
nmake
nmake install
```

#### Method 3: Manual Compilation

```cmd
cl /nologo /W3 /O2 /MD /Fe:suflow.exe suflow.c
```

### Build Options

- `/W3`: Warning level 3
- `/O2`: Optimize for speed
- `/MD`: Link with MSVCRT dynamically
- `/Fe:suflow.exe`: Output executable name

## Installation

After building, `suflow.exe` is automatically installed to:
```
<project_root>\bin\suflow.exe
```

Ensure this directory is in your PATH for convenient access.

## Integration

### SU Flow GUI

`suflow.exe` is used by the SU Flow GUI (`zau/src/pysu/gui/suflow_gui.py`) to execute pipeline workflows. The GUI automatically constructs `suflow` commands for multi-stage pipelines.

### Command Line

For interactive use, add `bin\` to your PATH or use full paths:
```cmd
bin\suflow.exe "sugain agc=1 | sufilter" input.su output.su
```

## Technical Details

### Limitations

- **Maximum Commands**: 32 (defined by `MAX_COMMANDS`)
- **Command Length**: 4096 characters (defined by `MAX_CMD_LEN`)
- **Windows Only**: This utility is Windows-specific and uses Windows API functions

### Implementation Notes

- Uses `GetTempPathA()` to determine temporary directory
- Uses process ID in temp file names to avoid collisions
- All file operations use binary mode (`_O_BINARY`)
- Commands are executed via `system()` with `cmd /c` wrapper
- Error handling: returns non-zero exit code on failure

### Source Code Structure

- **`suflow.c`**: Main source file (238 lines)
  - `main()`: Entry point, argument parsing
  - `parse_pipeline()`: Pipeline string parser
  - `run_pipeline()`: Pipeline executor
  - `print_usage()`: Help text

- **`build.bat`**: Windows build script
- **`Makefile`**: NMAKE-compatible makefile

## Troubleshooting

### Build Issues

**Error: `cl.exe not found`**
- Solution: Run from Visual Studio Developer Command Prompt
- Or manually set up environment:
  ```cmd
  call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
  ```

### Runtime Issues

**Pipeline fails with exit code**
- Check that all commands in the pipeline are valid
- Verify input file exists and is readable
- Ensure output directory is writable

**Temporary files not cleaned up**
- Check system temp directory permissions
- Verify process wasn't terminated abnormally
- Manually delete: `suflow_tmp_*.su` files

## Examples

### Example 1: Simple Gain
```cmd
suflow "bin\sugain.exe agc=1 wagc=0.1" input.su output.su
```

### Example 2: Filter Chain
```cmd
suflow "bin\sufilter.exe f=10,20,30,40 | bin\sugain.exe scale=0.5" data.su filtered.su
```

### Example 3: Header Processing
```cmd
suflow "bin\sushw.exe key=cdp a=100 | bin\sugethw.exe key=cdp,tracl" input.su
```

### Example 4: Complex Workflow
```cmd
suflow "bin\sugain.exe tpow=2 | bin\sunormalize.exe | bin\sufilter.exe f=5,10,20,25 | bin\sustack.exe" traces.su stack.su
```

## See Also

- **SU Flow GUI**: `zau/src/pysu/gui/suflow_gui.py` - Graphical interface for building SU workflows
- **SU Programs**: `src/su/main/` - Source code for SU processing programs
- **Python Plugins**: `zau/src/pysu/plugins/` - Python-based SU flow plugins

