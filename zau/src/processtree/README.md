# SU Process Tree

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

This directory contains the SU (Seismic Unix) process tree catalog in YAML format. The process tree provides a hierarchical organization of all SU programs, mapping them to their categories, source directories, and help documentation.

## Contents

- **`su_process_tree.yaml`**: Complete catalog of SU processes organized by category

## File Structure

The YAML file contains a hierarchical structure:

```yaml
processes:
  - category: "Category Name"
    directory: "source_directory_name"
    processes:
      - name: "processname"
        help_file: "processname.md"  # or null if no help file
```

### Categories

The process tree organizes SU programs into the following categories:

- **Amplitudes**: Amplitude correction and scaling programs
- **Attributes & Parameter Estimation**: Attribute extraction and parameter estimation
- **Convolution & Correlation**: Convolution and correlation operations
- **Data Compression**: Data compression utilities
- **Data Conversion**: Format conversion tools (SEGY, ASCII, etc.)
- **Datuming**: Datum correction programs
- **Deconvolution & Shaping**: Deconvolution and wavelet shaping
- **Dip Moveout (DMO)**: Dip moveout correction
- **Filters**: Filtering operations (frequency, dip, etc.)
- **Headers**: Header manipulation and extraction
- **Interpolation & Extrapolation**: Data interpolation/extrapolation
- **Migration & Inversion**: Migration and inversion algorithms
- **Multicomponent**: Multi-component processing
- **Noise**: Noise generation and removal
- **Operations**: General data operations
- **Picking**: Event picking utilities
- **Stacking**: Stacking operations
- **Statics**: Static correction programs
- **Stretching, Moveout & Resampling**: NMO, resampling, stretching
- **Supromax**: ProMAX integration tools
- **Synthetics, Waveforms & Test Patterns**: Synthetic data generation
- **Tapering**: Tapering operations
- **Transforms**: Transform operations (FFT, Radon, etc.)
- **Velocity Analysis**: Velocity analysis tools
- **Well Logs**: Well log processing
- **Windowing, Sorting & Muting**: Windowing, sorting, and muting operations

## Usage

### Generating the Process Tree

The process tree can be regenerated using the provided scripts:

```cmd
cd zau\scripts
python generate_process_tree_yaml.py
```

Or using the direct YAML creation script:

```cmd
cd zau\scripts
python create_yaml_direct.py
```

### Scripts

- **`zau/scripts/generate_process_tree_yaml.py`**: Main generator script
  - Scans `src/su/main/` directory structure
  - Maps processes to help files in `zau/doc/suman/`
  - Outputs to `zau/src/processtree/su_process_tree.yaml`

- **`zau/scripts/create_yaml_direct.py`**: Alternative generator
  - Creates YAML without requiring yaml library
  - Only includes processes with corresponding `.exe` files in `bin/`

### Integration

The process tree is used by:

- **SU Flow GUI**: For future process browser features
- **Help System**: Maps process names to markdown help files
- **Documentation**: Provides structured view of available SU programs

## Help Files

Each process entry may reference a help file in `zau/doc/suman/`:

- If a markdown help file exists: `help_file: "processname.md"`
- If no help file exists: `help_file: null`

Help files are located at:
```
zau/doc/suman/{processname}.md
```

## File Location

The process tree file is located at:
```
zau/src/processtree/su_process_tree.yaml
```

This location was chosen to:
- Keep process metadata separate from GUI code
- Allow easy access by multiple tools
- Maintain clear organization of project resources

## Updating the Process Tree

To update the process tree:

1. **After adding new SU programs:**
   ```cmd
   cd zau\scripts
   python generate_process_tree_yaml.py
   ```

2. **After building new executables:**
   ```cmd
   cd zau\scripts
   python create_yaml_direct.py
   ```

3. **After adding help documentation:**
   - Help files should be placed in `zau/doc/suman/`
   - Regenerate the process tree to update references

## See Also

- **SU Flow GUI**: `zau/src/pysu/gui/` - GUI that uses the process tree
- **Help Documentation**: `zau/doc/suman/` - Markdown help files for SU programs
- **SU Source Code**: `src/su/main/` - Source code for SU programs
- **Generation Scripts**: 
  - `zau/scripts/generate_process_tree_yaml.py` - Main process tree generator
  - `zau/scripts/create_yaml_direct.py` - Alternative generator (only includes processes with .exe files)

