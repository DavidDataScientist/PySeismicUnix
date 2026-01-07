# Third-Party Packages

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

This directory contains third-party packages that have been ported, updated, or modified for use with PySeismicUnix. These packages are included to support the build system and visualization capabilities of the project.

## Important Notice

**These packages have been modified for use with PySeismicUnix, but our changes have NOT been submitted to the respective upstream repositories or authors.**

**All copyright and license rights remain with the original owners and authors of these packages.**

## Included Packages

### SCons

**Location**: `scons/`

**Original Project**: SCons - A software construction tool  
**Original License**: MIT License  
**Copyright**: Copyright (c) 2001 - 2021 The SCons Foundation

SCons is used as the build system for PySeismicUnix demos that previously relied on SCons. We have not used it for anything going forward. The version included here may have been modified or updated for compatibility with our Windows build environment.

**Original Repository**: https://github.com/SCons/scons

### Seismic-Canvas

**Location**: `seismic-canvas/`

**Original Project**: Seismic-Canvas - Interactive 3D seismic visualization tool  
**Original License**: MIT License  
**Copyright**: Copyright (c) 2019-present Yunzhi Shi. All rights reserved.

Seismic-Canvas provides 3D visualization capabilities for seismic data using VisPy and OpenGL. The version included here may have been modified for integration with PySeismicUnix.

**Original Repository**: https://github.com/yunzhishi/seismic-canvas

## License Compliance

Each package retains its original license and copyright. Please refer to the individual LICENSE files in each package directory:

- `scons/LICENSE` - SCons license information
- `seismic-canvas/LICENSE` - Seismic-Canvas license information

## Modifications

Any modifications made to these packages are:

1. **For internal use only** - To support PySeismicUnix functionality
2. **Not submitted upstream** - Changes have not been contributed back to the original projects
3. **Compatible with original licenses** - All modifications comply with the original MIT licenses

## Usage

These packages are included as part of the PySeismicUnix distribution. They should be used in accordance with their original licenses:

- **SCons**: Used automatically by the build system
- **Seismic-Canvas**: Available for optional 3D visualization features

## Acknowledgments

We acknowledge and thank the original authors and contributors of these excellent open-source projects:

- **The SCons Foundation** - For providing a powerful and flexible build system
- **Yunzhi Shi** - For creating the Seismic-Canvas visualization tool

## See Also

- **PySeismicUnix Project**: Main project documentation
- **Build System**: `WINDOWS_BUILD_GUIDE.md` - Information about using SCons
- **Visualization**: `zau/src/pysu/gui/README.md` - GUI and visualization features

