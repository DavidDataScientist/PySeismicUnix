# SU Flow GUI - Seismic Unix Workflow Builder

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

The SU Flow GUI is a PyQt6-based graphical interface for building, testing, and executing SU (Seismic Unix) processing workflows on Windows. It provides an intuitive dark-mode interface for composing multi-stage SU pipelines and viewing results in real-time.

## Features

- **Dark Mode Interface**: Modern, eye-friendly dark theme optimized for extended use
- **Pipeline Builder**: Visual interface for composing SU command pipelines
- **Preset Workflows**: Pre-configured test workflows for common operations
- **Real-time Console**: Live output display with scrollable console window
- **File Browser**: Integrated file picker for selecting input data files
- **Binary Pipe Support**: Automatic integration with `suflow.exe` for reliable Windows binary data transfer
- **Threaded Execution**: Non-blocking workflow execution keeps GUI responsive
- **Error Handling**: Clear error messages and exit code reporting

## Requirements

### System Requirements

- **Windows 10/11** (64-bit)
- **Python 3.13.x** (or compatible 3.x version)
- **Visual Studio 2022** (for building SU programs and suflow.exe)

### Python Dependencies

The GUI requires the following Python packages (automatically installed during setup):

- **PyQt6** >= 6.4.0 - GUI framework
- **numpy** >= 1.19.0, < 2.0.0 - Numerical operations (for seismic-canvas)
- **vispy** >= 0.12.0 - 3D visualization (for seismic-canvas)
- **PyOpenGL** >= 3.1.0 - OpenGL bindings (for seismic-canvas)
- **matplotlib** >= 3.3.0, < 4.0.0 - Plotting (for seismic-canvas)

Note: The seismic-canvas dependencies are optional and only needed if you plan to use 3D visualization features.

## Installation

### Quick Setup

1. **Navigate to the GUI directory:**
   ```cmd
   cd zau\src\pysu
   ```

2. **Run the setup script:**
   ```cmd
   setup_gui.bat
   ```

   This script will:
   - Check for Python installation
   - Create a virtual environment (`venv/`)
   - Install all required dependencies
   - Verify installation

### Manual Setup

If you prefer manual setup:

```cmd
cd zau\src\pysu

REM Create virtual environment
python -m venv venv

REM Activate virtual environment
venv\Scripts\activate.bat

REM Install dependencies
pip install --upgrade pip
pip install -r requirements.txt
```

## Running the GUI

### Method 1: Using run_gui.bat (Recommended)

```cmd
cd zau\src\pysu
run_gui.bat
```

This script automatically:
- Checks for virtual environment
- Runs setup if needed
- Activates venv
- Launches the GUI

### Method 2: Direct Python Execution

```cmd
cd zau\src\pysu
venv\Scripts\activate.bat
python gui\suflow_gui.py
```

### Method 3: From GUI Directory

```cmd
cd zau\src\pysu\gui
..\..\run_gui.bat
```

## Usage

### Interface Overview

The GUI consists of several sections:

1. **Test Workflows Dropdown**: Select from pre-configured test workflows
2. **Input Data Section**: Specify input SU file (with browse button)
3. **SU Flow Pipeline**: Enter custom pipeline commands
4. **Console Output**: View real-time execution output
5. **Status Bar**: Shows project root and execution status

### Using Preset Workflows

1. Select a workflow from the "Test Workflows" dropdown:
   - **A - Basic Range Check**: Display header ranges
   - **B - Get Headers**: Extract specific header values
   - **C - AGC Gain + Range**: Apply AGC and show ranges
   - **D - Normalize + Scale**: Normalize and scale traces
   - **E - Gain Chain**: Time power correction and scaling
   - **F - Header Manipulation**: Set and display headers

2. The workflow automatically populates:
   - Flow pipeline command
   - Input file path
   - Description

3. Click **"▶ Run Flow"** to execute

### Creating Custom Workflows

1. **Enter Pipeline Commands:**
   - Type SU commands separated by `|` (pipe)
   - Example: `bin\sugain.exe agc=1 | bin\sufilter.exe f=10,20,80,100 | bin\surange.exe`

2. **Specify Input File:**
   - Enter path manually, or
   - Click **"Browse..."** to select file

3. **Run Workflow:**
   - Click **"▶ Run Flow"** button
   - Or press **Enter** in the pipeline field

4. **View Results:**
   - Console output appears in real-time
   - Exit code shown at completion
   - Status bar shows success/failure

### Pipeline Syntax

The GUI accepts standard SU pipeline syntax:

```cmd
bin\sugain.exe agc=1 wagc=0.1 | bin\sufilter.exe f=10,20,80,100 | bin\surange.exe
```

**Key Points:**
- Use `|` to separate commands
- Include `bin\` prefix for SU executables
- Use Windows-style backslashes in paths
- Parameters use `key=value` format
- Multiple parameters separated by spaces

### Example Workflows

**Simple Gain:**
```
bin\sugain.exe agc=1 wagc=0.1
```

**Filter Chain:**
```
bin\sufilter.exe f=10,20,30,40 | bin\sugain.exe scale=0.5
```

**Complex Processing:**
```
bin\sugain.exe tpow=2 | bin\sunormalize.exe | bin\sufilter.exe f=5,10,20,25 | bin\sustack.exe
```

**Header Operations:**
```
bin\sushw.exe key=cdp a=100 | bin\sugethw.exe key=cdp,tracl
```

## Architecture

### Components

- **`suflow_gui.py`**: Main GUI application (488 lines)
  - `SUFlowGUI`: Main window class
  - `WorkerThread`: Background execution thread
  - `apply_dark_theme()`: Theme configuration

- **`su_process_tree.yaml`**: Process catalog (located in `zau/src/processtree/`)
  - Categorized list of SU programs
  - Help file references
  - Used for future process browser features

- **`run_gui.bat`**: Launcher script
- **`setup_gui.bat`**: Setup automation script

### Integration with suflow.exe

The GUI automatically uses `suflow.exe` for pipeline execution:

1. **Pipeline Detection**: If pipeline contains `|`, uses `suflow.exe`
2. **Command Construction**: Builds `suflow "pipeline" input.su` command
3. **Execution**: Runs via `subprocess.Popen()` in background thread
4. **Output Streaming**: Captures stdout/stderr in real-time

### Project Root Detection

The GUI automatically finds the project root by:
1. Looking for `bin/suflow.exe` in parent directories (up to 10 levels)
2. Falling back to `CWPROOT` environment variable
3. Defaulting to current directory

## Configuration

### Default Input File

The GUI defaults to:
```
<project_root>/src/su/tutorial/data.su
```

Change this by:
- Entering a new path in the Input File field
- Using the Browse button
- Selecting a preset workflow (which sets its own input)

### suflow.exe Location

The GUI expects `suflow.exe` at:
```
<project_root>/bin/suflow.exe
```

If not found, the GUI will show an error when attempting to run workflows.

## Troubleshooting

### GUI Won't Start

**Error: "Python not found"**
- Solution: Install Python 3.13.x from python.org
- Ensure Python is in PATH

**Error: "PyQt6 not found"**
- Solution: Run `setup_gui.bat` to install dependencies
- Or manually: `pip install PyQt6`

**Error: "ModuleNotFoundError"**
- Solution: Ensure virtual environment is activated
- Run: `venv\Scripts\activate.bat`

### Workflow Execution Issues

**Error: "suflow.exe not found"**
- Solution: Build `suflow.exe` first:
  ```cmd
  cd zau\src\suflow
  build.bat
  ```
- Verify `bin\suflow.exe` exists

**Error: "Input file not found"**
- Solution: Check input file path is correct
- Use Browse button to select file
- Ensure file exists and is readable

**Pipeline fails with exit code**
- Check console output for error messages
- Verify all SU programs in pipeline are built
- Ensure pipeline syntax is correct
- Check that input file is valid SU format

### Display Issues

**GUI appears too small/large**
- Resize window manually
- Minimum size: 900x700 pixels

**Console output not visible**
- Scroll down in console window
- Click "Clear Console" to reset
- Check that workflow actually ran

**Dark theme not applied**
- This is normal on some Windows themes
- GUI uses Fusion style with custom palette
- Colors may vary slightly by system

## Advanced Usage

### Custom Python Environment

If you prefer using a system Python or different environment:

```cmd
REM Activate your environment
your_env\Scripts\activate.bat

REM Install dependencies
pip install -r requirements.txt

REM Run GUI
python gui\suflow_gui.py
```

### Integration with Other Tools

The GUI can be extended to:
- Save/load workflow configurations
- Export workflows as batch scripts
- Integrate with process help system (see `zau/src/processtree/su_process_tree.yaml`)
- Add visualization capabilities using seismic-canvas

### Extending Preset Workflows

Edit `suflow_gui.py` to add more presets:

```python
TEST_WORKFLOWS = {
    "Your Workflow Name": {
        "flow": "bin\\your_command.exe param=value",
        "description": "What this workflow does",
        "input": "path/to/input.su"
    },
    # ... more workflows
}
```

## See Also

- **suflow.exe**: `zau/src/suflow/README.md` - Binary pipe wrapper documentation
- **SU Programs**: `src/su/main/` - Source code for SU processing programs
- **Python Plugins**: `zau/src/pysu/plugins/` - Python-based SU flow plugins
- **Process Tree**: `zau/src/processtree/su_process_tree.yaml` - SU process catalog

## Development Notes

### Code Structure

- **Main Window**: `SUFlowGUI` class handles all UI and workflow logic
- **Worker Thread**: `WorkerThread` executes workflows without blocking UI
- **Theme**: `apply_dark_theme()` configures dark mode palette
- **Signals**: PyQt6 signals used for thread-safe UI updates

### Future Enhancements

Potential improvements:
- Process browser with help integration
- Workflow save/load functionality
- Output file selection
- Pipeline validation
- Syntax highlighting for pipeline editor
- 3D visualization integration (seismic-canvas)

