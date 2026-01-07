# UNIX Backport Status

## Current Status

**As of this writing, we have not yet attempted to backport the non-Windows related changes (including other SU fixes and Python support) to Linux or macOS platforms.**

This document provides a potential pathway for anyone who wishes to attempt this backporting effort.

## Overview of Changes to Backport

The following categories of changes may need to be backported:

1. **SU (Seismic Unix) Fixes**: Various bug fixes and improvements to the SU processing tools
2. **Python Support**: Python integration and scripting capabilities added to the project

## Potential Backport Pathway

### Phase 1: Assessment and Preparation

1. **Identify Platform-Specific Code**
   - Review the codebase for Windows-specific implementations (e.g., path separators, file handling)
   - Document any conditional compilation or platform checks
   - Identify shared/unified code that should work across platforms

2. **Review Build System**
   - Examine build scripts and configuration files
   - Identify Windows-specific build steps (e.g., `.bat`, `.ps1` scripts)
   - Note any dependencies that may differ between platforms

3. **Document Dependencies**
   - List all external dependencies
   - Verify availability on target platforms (Linux/macOS)
   - Check version compatibility requirements

### Phase 2: Code Adaptation

1. **Path Handling**
   - Replace Windows path separators (`\`) with platform-agnostic approaches
   - Use `os.path.join()` or `pathlib` in Python code
   - Ensure SU tools handle paths correctly on Unix-like systems

2. **File System Operations**
   - Review file I/O operations for platform compatibility
   - Check file permission handling
   - Verify line ending handling (CRLF vs LF) if relevant

3. **Build System Adaptation**
   - Create Unix-compatible build scripts (shell scripts for Linux/macOS)
   - Adapt Makefiles or build configurations as needed
   - Ensure compiler flags are appropriate for each platform

### Phase 3: Python Integration

1. **Python Environment**
   - Verify Python version compatibility (project uses Python >= 3.13)
   - Ensure virtual environment setup works on Unix systems
   - Test Python module imports and dependencies

2. **Script Compatibility**
   - Review Python scripts for platform-specific code
   - Ensure subprocess calls work on Unix systems
   - Verify any system command invocations

3. **Testing**
   - Create test cases that validate Python integration
   - Test on both Linux and macOS if possible
   - Verify SU tool invocation from Python works correctly

### Phase 4: Testing and Validation

1. **Platform-Specific Testing**
   - Test on target Linux distribution(s)
   - Test on macOS (if applicable)
   - Verify all SU tools function correctly

2. **Integration Testing**
   - Test Python integration end-to-end
   - Verify build process works on Unix systems
   - Test example workflows and use cases

3. **Documentation Updates**
   - Update installation instructions for Unix platforms
   - Document any platform-specific requirements
   - Update build instructions

### Phase 5: Build Scripts and Automation

1. **Create Unix Build Scripts**
   - Develop shell scripts equivalent to Windows `.bat`/`.ps1` scripts
   - Place in `scripts/` directory following project conventions
   - Ensure scripts handle virtual environment activation correctly

2. **CI/CD Considerations**
   - If applicable, add Unix platform testing to CI/CD pipeline
   - Ensure automated builds work on target platforms

## Key Considerations

### Differences to Watch For

- **Path Separators**: Windows uses `\`, Unix uses `/`
- **Executable Extensions**: Windows uses `.exe`, Unix typically has no extension
- **Line Endings**: May need to handle CRLF vs LF differences
- **Library Paths**: Different conventions for shared libraries (.dll vs .so/.dylib)
- **Shell Differences**: PowerShell vs bash/zsh

### Tools and Resources

- Use `os.path` or `pathlib` for cross-platform path handling
- Consider using `platform` module in Python for platform detection
- Review SU documentation for Unix-specific build requirements
- Test incrementally on each platform

## Getting Started

If you're interested in attempting this backport:

1. Start with a clean checkout of the current codebase
2. Set up a Linux or macOS development environment
3. Begin with Phase 1 (Assessment) to understand the scope
4. Work incrementally, testing as you go
5. Document any issues or platform-specific quirks you discover

## Notes

- This is a **potential pathway** - actual implementation may reveal additional considerations
- Some changes may be straightforward, while others may require more significant adaptation
- Consider maintaining platform-specific branches or using conditional compilation where appropriate
- Community contributions and testing would be valuable for this effort

## Contributing

If you successfully backport these changes, please consider:
- Documenting your process and any challenges encountered
- Contributing back to the project
- Sharing platform-specific build scripts or configurations
- Updating this document with your findings

---

*This document is a guide for potential backporting efforts. Actual implementation may vary based on specific requirements and discoveries during the process.*

