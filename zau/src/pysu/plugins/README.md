# SU Flow Python Plugins

This directory contains Python plugins for SU (Seismic Unix) flow pipelines.

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

Plugins allow Python code to be integrated into SU command pipelines, enabling custom processing steps written in Python.

## Architecture

### Base Classes

- **`SuFlowPlugin`**: Abstract base class for all plugins
- **`SuTrace`**: Data class representing a single SU trace (header + data)

### Factory Pattern

- **`SuPluginFactory`**: Registers and creates plugin instances
- Plugins are registered by name and can be instantiated dynamically

## Usage

### Command Line

```bash
# Basic usage
python -m zau.src.pysu.plugins <plugin_name> < input.su > output.su

# In SU flow pipeline
bin\sugain.exe agc=1 wagc=0.1 | python -m zau.src.pysu.plugins pysuflipsign | bin\surange.exe
```

### Available Plugins

- **`pysuflipsign`**: Flips the sign of all trace samples (test plugin)

## Creating a New Plugin

1. Create a new Python file in this directory
2. Subclass `SuFlowPlugin` and implement `process_trace()`
3. Register the plugin using `register_plugin()`

Example:

```python
from .base import SuFlowPlugin, SuTrace
from .factory import register_plugin

class MyPlugin(SuFlowPlugin):
    def process_trace(self, trace: SuTrace) -> SuTrace:
        # Process trace.data here
        trace.data = [x * 2.0 for x in trace.data]
        return trace

register_plugin('myplugin', MyPlugin)
```

## SU Format

- **Header**: 240 bytes (HDRBYTES)
- **Data**: `ns` samples of 32-bit floats (4 bytes each)
- **ns**: Number of samples, stored in header at offset 72 (4 bytes, big-endian int)

## Notes

- Plugins read from `stdin` and write to `stdout` (binary mode)
- Headers are preserved and passed through (unless modified)
- Plugins can skip traces by returning `None` from `process_trace()`
- Use `initialize()` for setup and `finalize()` for cleanup


