# Python Integration Guide

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

PySeismicUnix provides comprehensive Python integration for extending and enhancing SU (Seismic Unix) workflows. This guide covers all Python-related features, including plugins, GUI tools, and development workflows.

**Python Version:** 3.13.x (or compatible 3.x)

---

## Table of Contents

1. [Python Plugin System](#python-plugin-system)
2. [SU Flow GUI](#su-flow-gui)
3. [Development Tools](#development-tools)
4. [Installation & Setup](#installation--setup)
5. [Workflow Examples](#workflow-examples)
6. [Advanced Topics](#advanced-topics)

---

## Python Plugin System

### Overview

The Python plugin system allows you to write custom seismic processing algorithms in Python and integrate them directly into SU pipelines. Plugins follow a simple interface and can leverage Python's rich ecosystem (NumPy, SciPy, etc.).

### Architecture

- **`pysurun.exe`**: Bridge program that runs Python plugins in SU pipelines
- **Plugin Base Classes**: `SuFlowPlugin` abstract base class
- **Factory Pattern**: Automatic plugin registration and discovery
- **Module Path**: `zau.src.pysu.plugins` (default)

### Using Plugins

#### Basic Usage

```cmd
REM Process data with a Python plugin
pysurun.exe plugin=pysuflipsign < input.su > output.su

REM In a pipeline (using suflow.exe)
suflow.exe "sugain agc=1 | pysurun plugin=pysuflipsign | surange"
```

#### Parameters

- **`plugin=name`** (required) - Name of the Python plugin to execute
- **`python=python`** (optional) - Python interpreter command (default: `python`)
- **`venv=path`** (optional) - Path to virtual environment
  - Uses `venv\Scripts\python.exe` (Windows) or `venv/bin/python` (Unix)
  - Takes precedence over `python=` if both specified
- **`module=zau.src.pysu.plugins`** (optional) - Python module path
- **`verbose=0`** (optional) - Set to `1` for verbose output

#### Examples

```cmd
REM Basic plugin execution
pysurun.exe plugin=pysuflipsign < input.su > output.su

REM With virtual environment
pysurun.exe plugin=pysuflipsign venv=D:\projects\myproject\venv < input.su > output.su

REM In complex pipeline
suflow.exe "suplane | pysurun plugin=pysuflipsign | sugain agc=1 | sufilter f=10,20,40,50"

REM With custom Python interpreter
pysurun.exe plugin=pysuflipsign python=python3 < input.su > output.su

REM Verbose mode
pysurun.exe plugin=pysuflipsign verbose=1 < input.su > output.su
```

### Available Plugins

#### pysuflipsign

Flips the sign of all samples in each trace (multiplies by -1).

```cmd
pysurun.exe plugin=pysuflipsign < input.su > output.su
```

### Creating Custom Plugins

#### Plugin Template

Create a new Python file in `zau/src/pysu/plugins/`:

```python
"""
My Custom SU Plugin

This plugin performs custom processing on SU traces.
"""

from .base import SuFlowPlugin, SuTrace
from .factory import register_plugin

class MyCustomPlugin(SuFlowPlugin):
    """
    Custom plugin that processes SU traces.
    """
    
    def process_trace(self, trace: SuTrace) -> SuTrace:
        """
        Process a single trace.
        
        Args:
            trace: Input trace with header and data
            
        Returns:
            Processed trace, or None to skip
        """
        # Example: Scale all samples by 2.0
        trace.data = [x * 2.0 for x in trace.data]
        
        # Example: Modify header
        # trace.set_header_int(offset, value)
        
        return trace
    
    def initialize(self) -> None:
        """Called once before processing any traces."""
        # Setup code here (optional)
        pass
    
    def finalize(self) -> None:
        """Called once after all traces are processed."""
        # Cleanup code here (optional)
        pass

# Register the plugin
register_plugin('myplugin', MyCustomPlugin)
```

#### Plugin Interface

**`SuFlowPlugin`** abstract base class:

- **`process_trace(trace: SuTrace) -> SuTrace | None`** (required)
  - Process a single trace
  - Return processed trace or `None` to skip
  
- **`initialize() -> None`** (optional)
  - Called once before processing
  
- **`finalize() -> None`** (optional)
  - Called once after processing

**`SuTrace`** data class:

- **`header: bytes`** - 240-byte SU trace header
- **`data: list[float]`** - Trace samples as floats
- **`ns: int`** - Number of samples (property)
- **`get_header_int(offset: int) -> int`** - Read integer from header
- **`set_header_int(offset: int, value: int) -> None`** - Write integer to header
- **`get_header_float(offset: int) -> float`** - Read float from header
- **`set_header_float(offset: int, value: float) -> None`** - Write float to header
- **`to_bytes() -> bytes`** - Convert trace to binary SU format

#### SU Format Details

- **Header**: 240 bytes (HDRBYTES)
- **Data**: `ns` samples of 32-bit floats (4 bytes each)
- **ns**: Number of samples, stored in header at offset 72 (4 bytes, big-endian int)

#### Plugin Registration

Plugins are automatically registered when imported. The plugin module (`zau/src/pysu/plugins/__main__.py`) imports all plugin files to trigger registration.

#### Advanced Plugin Examples

**Using NumPy:**

```python
import numpy as np
from .base import SuFlowPlugin, SuTrace
from .factory import register_plugin

class NumPyPlugin(SuFlowPlugin):
    def process_trace(self, trace: SuTrace) -> SuTrace:
        # Convert to NumPy array
        data = np.array(trace.data)
        
        # Process with NumPy
        data = np.abs(data)  # Example: absolute value
        
        # Convert back
        trace.data = data.tolist()
        return trace

register_plugin('numpyabs', NumPyPlugin)
```

**Skipping Traces:**

```python
class FilterPlugin(SuFlowPlugin):
    def process_trace(self, trace: SuTrace) -> SuTrace:
        # Skip traces with zero samples
        if trace.ns == 0:
            return None  # Skip this trace
        
        # Process valid traces
        trace.data = [x * 2.0 for x in trace.data]
        return trace

register_plugin('filter', FilterPlugin)
```

**Header Manipulation:**

```python
class HeaderPlugin(SuFlowPlugin):
    def process_trace(self, trace: SuTrace) -> SuTrace:
        # Read header value
        tracl = trace.get_header_int(0)  # Trace number at offset 0
        
        # Modify header
        trace.set_header_int(0, tracl + 1000)
        
        # Process data
        trace.data = [x * 1.5 for x in trace.data]
        return trace

register_plugin('headermod', HeaderPlugin)
```

### Plugin Development Workflow

1. **Create plugin file** in `zau/src/pysu/plugins/`
2. **Implement `SuFlowPlugin`** subclass
3. **Register plugin** with `register_plugin()`
4. **Test directly:**
   ```cmd
   python -m zau.src.pysu.plugins myplugin < input.su > output.su
   ```
5. **Test with pysurun:**
   ```cmd
   pysurun.exe plugin=myplugin < input.su > output.su
   ```

### Plugin Location

By default, plugins are loaded from:
```
zau/src/pysu/plugins/
```

You can specify a custom module path:
```cmd
pysurun.exe plugin=myplugin module=my.custom.plugins < input.su > output.su
```

---

## SU Flow GUI

### Overview

The SU Flow GUI is a PyQt6-based graphical interface for building, testing, and executing SU processing workflows. It provides an intuitive dark-mode interface for composing multi-stage SU pipelines.

**Location:** `zau/src/pysu/gui/suflow_gui.py`

### Features

- **Dark Mode Interface**: Modern, eye-friendly dark theme
- **Pipeline Builder**: Visual interface for composing SU command pipelines
- **Preset Workflows**: 6 pre-configured test workflows
- **Real-time Console**: Live output display with scrollable console
- **File Browser**: Integrated file picker for input data
- **Binary Pipe Support**: Automatic integration with `suflow.exe`
- **Threaded Execution**: Non-blocking workflow execution

### Installation

#### Quick Setup

```cmd
cd zau\src\pysu
setup_gui.bat
```

This script:
- Checks for Python installation
- Creates virtual environment (`venv/`)
- Installs all required dependencies
- Verifies installation

#### Manual Setup

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

### Running the GUI

```cmd
cd zau\src\pysu
run_gui.bat
```

Or directly:
```cmd
cd zau\src\pysu
venv\Scripts\activate.bat
python gui\suflow_gui.py
```

### Usage

#### Interface Overview

1. **Test Workflows Dropdown**: Select from pre-configured workflows
2. **Input Data Section**: Specify input SU file (with browse button)
3. **SU Flow Pipeline**: Enter custom pipeline commands
4. **Console Output**: View real-time execution output
5. **Status Bar**: Shows project root and execution status

#### Preset Workflows

- **A - Basic Range Check**: Display header ranges
- **B - Get Headers**: Extract specific header values
- **C - AGC Gain + Range**: Apply AGC and show ranges
- **D - Normalize + Scale**: Normalize and scale traces
- **E - Gain Chain**: Time power correction and scaling
- **F - Header Manipulation**: Set and display headers

#### Creating Custom Workflows

1. Enter pipeline commands separated by `|`:
   ```
   bin\sugain.exe agc=1 | bin\sufilter.exe f=10,20,80,100 | bin\surange.exe
   ```

2. Specify input file (enter path or use Browse button)

3. Click **"▶ Run Flow"** or press Enter

4. View results in console output

### Requirements

- **Python 3.13.x** (or compatible 3.x)
- **PyQt6** >= 6.4.0
- **Optional**: numpy, vispy, PyOpenGL, matplotlib (for seismic-canvas 3D visualization)

---

## Development Tools

### Process Tree Generation

Python scripts for generating the SU process catalog:

#### `zau/scripts/generate_process_tree_yaml.py`

Main generator script that scans `src/su/main/` and creates YAML catalog.

**Usage:**
```cmd
cd zau\scripts
python generate_process_tree_yaml.py
```

**Output:** `zau/src/processtree/su_process_tree.yaml`

#### `zau/scripts/create_yaml_direct.py`

Alternative generator that only includes processes with corresponding `.exe` files.

**Usage:**
```cmd
cd zau\scripts
python create_yaml_direct.py
```

**Features:**
- No yaml library required
- Only includes built executables
- Direct YAML generation

---

## Installation & Setup

### Python Environment

#### System Python

If using system Python:

```cmd
REM Install dependencies globally
pip install PyQt6

REM For GUI
cd zau\src\pysu
pip install -r requirements.txt
```

#### Virtual Environment (Recommended)

**For GUI:**
```cmd
cd zau\src\pysu
setup_gui.bat
```

**For Plugins:**
```cmd
REM Create venv in project root or custom location
python -m venv my_venv
my_venv\Scripts\activate.bat
pip install -r zau\src\pysu\requirements.txt
```

### Dependencies

#### Required

- **PyQt6** >= 6.4.0 - GUI framework

#### Optional (for 3D visualization)

- **numpy** >= 1.19.0, < 2.0.0 - Numerical operations
- **vispy** >= 0.12.0 - 3D visualization
- **PyOpenGL** >= 3.1.0 - OpenGL bindings
- **matplotlib** >= 3.3.0, < 4.0.0 - Plotting

**Install all:**
```cmd
pip install PyQt6 numpy "vispy>=0.12.0" PyOpenGL "matplotlib>=3.3.0,<4.0.0"
```

### Project Structure

```
zau/
├── src/
│   └── pysu/
│       ├── plugins/          # Python plugin system
│       │   ├── base.py       # SuFlowPlugin, SuTrace
│       │   ├── factory.py    # Plugin registration
│       │   ├── __main__.py   # Command-line entry point
│       │   └── *.py          # Plugin implementations
│       ├── gui/              # SU Flow GUI
│       │   └── suflow_gui.py # Main GUI application
│       ├── requirements.txt  # Python dependencies
│       ├── setup_gui.bat     # GUI setup script
│       └── run_gui.bat      # GUI launcher
└── scripts/                  # Development tools
    ├── generate_process_tree_yaml.py
    └── create_yaml_direct.py
```

---

## Workflow Examples

### Example 1: Basic Plugin Workflow

```cmd
REM 1. Create a simple plugin
REM    (Edit zau/src/pysu/plugins/myplugin.py)

REM 2. Test the plugin directly
python -m zau.src.pysu.plugins myplugin < input.su > output.su

REM 3. Use in SU pipeline
suflow.exe "sugain agc=1 | pysurun plugin=myplugin | surange"
```

### Example 2: GUI Workflow Development

```cmd
REM 1. Launch GUI
cd zau\src\pysu
run_gui.bat

REM 2. Select preset workflow or enter custom pipeline
REM 3. Select input file
REM 4. Click "Run Flow"
REM 5. Review console output
```

### Example 3: Plugin with NumPy

```python
# Plugin using NumPy for advanced processing
import numpy as np
from .base import SuFlowPlugin, SuTrace
from .factory import register_plugin

class NumPyFilterPlugin(SuFlowPlugin):
    def process_trace(self, trace: SuTrace) -> SuTrace:
        data = np.array(trace.data)
        
        # Apply NumPy operations
        data = np.convolve(data, [0.25, 0.5, 0.25], mode='same')
        
        trace.data = data.tolist()
        return trace

register_plugin('numpyfilter', NumPyFilterPlugin)
```

**Usage:**
```cmd
pysurun plugin=numpyfilter < input.su > output.su
```

### Example 4: Complex Pipeline with Python

```cmd
REM Pipeline mixing SU programs and Python plugins
suflow.exe "suplane | sugain agc=1 | pysurun plugin=myplugin | sufilter f=10,20,40,50 | surange"
```

### Example 5: Virtual Environment Workflow

```cmd
REM 1. Create venv with custom dependencies
python -m venv my_venv
my_venv\Scripts\activate.bat
pip install numpy scipy

REM 2. Use venv with pysurun
pysurun plugin=myplugin venv=my_venv < input.su > output.su
```

---

## Advanced Topics

### Plugin Performance

**Tips for optimal performance:**

1. **Use NumPy** for array operations instead of Python lists
2. **Batch processing** if possible (process multiple traces in one call)
3. **Avoid unnecessary copies** of trace data
4. **Use `initialize()`** for one-time setup (loading models, etc.)

**Example:**
```python
class OptimizedPlugin(SuFlowPlugin):
    def __init__(self, args=None):
        super().__init__(args)
        self.filter_coeffs = None
    
    def initialize(self):
        # Load filter coefficients once
        import numpy as np
        self.filter_coeffs = np.array([0.25, 0.5, 0.25])
    
    def process_trace(self, trace: SuTrace) -> SuTrace:
        import numpy as np
        data = np.array(trace.data)
        data = np.convolve(data, self.filter_coeffs, mode='same')
        trace.data = data.tolist()
        return trace
```

### Error Handling

Plugins should handle errors gracefully:

```python
class RobustPlugin(SuFlowPlugin):
    def process_trace(self, trace: SuTrace) -> SuTrace:
        try:
            # Process trace
            if trace.ns == 0:
                return None  # Skip empty traces
            
            trace.data = [x * 2.0 for x in trace.data]
            return trace
        except Exception as e:
            # Log error and skip trace
            import sys
            sys.stderr.write(f"Error processing trace: {e}\n")
            return None  # Skip this trace
```

### Integration with External Libraries

**Using SciPy:**
```python
from scipy import signal
from .base import SuFlowPlugin, SuTrace
from .factory import register_plugin

class SciPyPlugin(SuFlowPlugin):
    def process_trace(self, trace: SuTrace) -> SuTrace:
        import numpy as np
        data = np.array(trace.data)
        
        # Use SciPy signal processing
        data = signal.savgol_filter(data, window_length=5, polyorder=2)
        
        trace.data = data.tolist()
        return trace

register_plugin('scipyfilter', SciPyPlugin)
```

### Debugging Plugins

**Verbose mode:**
```cmd
pysurun plugin=myplugin verbose=1 < input.su > output.su
```

**Direct Python execution:**
```cmd
python -m zau.src.pysu.plugins myplugin < input.su > output.su
```

**Add debug output:**
```python
class DebugPlugin(SuFlowPlugin):
    def process_trace(self, trace: SuTrace) -> SuTrace:
        import sys
        sys.stderr.write(f"Processing trace with {trace.ns} samples\n")
        # ... processing ...
        return trace
```

### Custom Module Paths

If you have plugins in a different location:

```cmd
pysurun plugin=myplugin module=my.custom.plugins < input.su > output.su
```

The module must:
- Be importable by Python
- Have plugins registered via `register_plugin()`
- Follow the `SuFlowPlugin` interface

---

## Troubleshooting

### Plugin Not Found

**Error:** `Unknown plugin 'myplugin'`

**Solutions:**
- Ensure plugin file is in `zau/src/pysu/plugins/`
- Check that plugin is registered with `register_plugin()`
- Verify module path with `module=` parameter
- Check that plugin file is imported in `__main__.py`

### Python Not Found

**Error:** `python: command not found`

**Solutions:**
- Ensure Python is in PATH
- Use `python=` parameter: `pysurun plugin=myplugin python=python3`
- Use `venv=` parameter to specify virtual environment

### Import Errors

**Error:** `ModuleNotFoundError: No module named 'zau'`

**Solutions:**
- Ensure project root is in Python path
- Install plugin package: `pip install -e .` (if setup.py exists)
- Use absolute imports or adjust PYTHONPATH

### Virtual Environment Issues

**Error:** `venv\Scripts\python.exe not found`

**Solutions:**
- Verify venv path is correct
- Use forward slashes or escaped backslashes in path
- Ensure virtual environment is properly created

### GUI Won't Start

**Solutions:**
- Run `setup_gui.bat` to create/update virtual environment
- Check Python version: `python --version`
- Verify PyQt6 installation: `pip list | findstr PyQt6`
- Check for error messages in console

### Plugin Execution Fails

**Solutions:**
- Test plugin directly: `python -m zau.src.pysu.plugins myplugin`
- Enable verbose mode: `pysurun plugin=myplugin verbose=1`
- Check plugin code for errors
- Verify input data format (valid SU file)

---

## Best Practices

### Plugin Development

1. **Keep plugins focused** - One plugin, one purpose
2. **Handle edge cases** - Empty traces, invalid data, etc.
3. **Use type hints** - Improves code clarity
4. **Document plugins** - Add docstrings and comments
5. **Test thoroughly** - Test with various input data

### Performance

1. **Use NumPy** for numerical operations
2. **Minimize data copies** - Work in-place when possible
3. **Cache expensive operations** - Use `initialize()` for setup
4. **Profile plugins** - Identify bottlenecks

### Code Organization

1. **Group related plugins** - Organize by functionality
2. **Share utilities** - Create common helper modules
3. **Version plugins** - Track plugin versions
4. **Maintain compatibility** - Don't break existing workflows

---

## See Also

- **Plugin System:** `zau/src/pysu/plugins/README.md` - Detailed plugin documentation
- **GUI Documentation:** `zau/src/pysu/gui/README.md` - GUI user guide
- **User Guide:** `USER_GUIDE.md` - General SU usage guide
- **Windows Build Guide:** `WINDOWS_BUILD_GUIDE.md` - Building from source
- **Windows Compatibility:** `include/win32_compat/README.md` - Windows compatibility layer
- **Header Modifications:** `include/README.md` - Windows port header changes

---

## Quick Reference

### Plugin Commands

```cmd
# Basic usage
pysurun plugin=name < input.su > output.su

# With virtual environment
pysurun plugin=name venv=./venv < input.su > output.su

# In pipeline
suflow "sugain | pysurun plugin=name | surange"

# Direct Python execution
python -m zau.src.pysu.plugins name < input.su > output.su
```

### GUI Commands

```cmd
# Setup
cd zau\src\pysu
setup_gui.bat

# Run
run_gui.bat
```

### Development Commands

```cmd
# Generate process tree
cd zau\scripts
python generate_process_tree_yaml.py
```

---

*Happy Python processing!*

