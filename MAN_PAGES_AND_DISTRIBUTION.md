# Man Pages and Distribution Build Instructions

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

**Date:** December 5, 2022  
**Purpose:** Step-by-step guide for regenerating man pages and creating the distribution package

---

## Overview

This guide explains how to:
1. Regenerate Unix man pages from SU executables
2. Create a complete distribution package with all components

---

## Prerequisites

- **Python 3.x** installed and in PATH
- **Built SU executables** in `bin/` directory
- **Visual Studio 2022** environment (for distribution build scripts)

---

## Part 1: Regenerating Man Pages

### Quick Start

From the project root directory:

```batch
zau\tools\run_man_generator.bat
```

This will:
- Scan all `.exe` files in `bin/`
- Extract help text from each program
- Generate Unix man pages in `man/man1/`
- Generate Markdown documentation in `zau/doc/suman/`

### Manual Method

If you prefer to run the Python script directly:

```batch
cd zau\tools
python generate_man_pages.py
```

### Testing a Single Program

To test man page generation for a specific program:

```batch
cd zau\tools
python generate_man_pages.py <program_name>
```

Example:
```batch
python generate_man_pages.py sugain
```

### Output Locations

After running the generator:

- **Man pages**: `man/man1/{program}.1` (groff/troff format)
- **Markdown docs**: `zau/doc/suman/{program}.md`

### What Gets Generated

For each executable that provides help output:
1. **Unix man page** (`.1` file) - Traditional Unix manual page format
2. **Markdown documentation** (`.md` file) - Human-readable documentation

Programs that don't output help text (exit code ≠ 0) are skipped.

### Verification

After generation, check the output:

```batch
dir man\man1\*.1
dir zau\doc\suman\*.md
```

You should see matching counts (one `.1` file per `.md` file for each program).

---

## Part 2: Creating the Distribution Package

### Quick Start

From the project root directory:

```batch
zau\scripts\create_distribution.bat
```

This will:
- Create a clean distribution directory
- Copy all executables, libraries, headers, and documentation
- Include man pages (if generated)
- Create a ZIP archive

### What Gets Included

The distribution package includes:

| Component | Source | Destination |
|-----------|--------|-------------|
| Executables | `bin/*.exe` | `dist/cwpsu-windows-x64/bin/` |
| Libraries | `lib/*.lib` | `dist/cwpsu-windows-x64/lib/` |
| Headers | `include/*.h` | `dist/cwpsu-windows-x64/include/` |
| Documentation | `documents/*.md` | `dist/cwpsu-windows-x64/doc/` |
| Man Pages | `man/` | `dist/cwpsu-windows-x64/man/` |
| Sample Data | `src/su/tutorial/*.su` | `dist/cwpsu-windows-x64/samples/` |
| GUI | `zau/src/pysu/gui/*` | `dist/cwpsu-windows-x64/gui/` |

### Distribution Structure

```
dist/cwpsu-windows-x64/
├── bin/          # All SU executables
├── lib/          # Static libraries
├── include/      # Header files
├── doc/          # User guides and documentation
├── man/          # Unix man pages (man1/)
├── samples/      # Sample SU data files
├── gui/          # PyQt6 SU Flow GUI
└── README.txt    # Distribution README
```

### Distribution Summary

After running the script, you'll see a summary:

```
========================================
Distribution Summary
========================================
  Executables: 312
  Libraries:   9
  Man pages:   47
  Location:    D:\src\proto\processing-su-main\dist\cwpsu-windows-x64
========================================
```

### ZIP Archive

The script automatically creates a ZIP archive:

```
dist/cwpsu-windows-x64.zip
```

This ZIP file contains the entire distribution and can be shared or deployed.

---

## Complete Workflow

### Full Build and Distribution Process

1. **Build all SU programs** (if not already built):
   ```batch
   zau\scripts\build_all_su_main.bat
   ```

2. **Regenerate man pages**:
   ```batch
   zau\tools\run_man_generator.bat
   ```

3. **Create distribution**:
   ```batch
   zau\scripts\create_distribution.bat
   ```

4. **Verify distribution**:
   ```batch
   dir dist\cwpsu-windows-x64\bin\*.exe
   dir dist\cwpsu-windows-x64\man\man1\*.1
   ```

---

## Troubleshooting

### Man Page Generation Issues

**Problem:** "Cannot find bin directory"
- **Solution:** Run from project root or ensure `bin/` directory exists

**Problem:** No man pages generated
- **Solution:** Check that executables provide help output (run `bin\sugain.exe --help` to test)

**Problem:** Man pages created in wrong location
- **Solution:** Ensure you're running from project root, not from a subdirectory

### Distribution Build Issues

**Problem:** Distribution directory not created
- **Solution:** Check that `CWPROOT` path in script matches your project location

**Problem:** Man pages not included
- **Solution:** Run `zau\tools\run_man_generator.bat` first to generate man pages

**Problem:** ZIP creation fails
- **Solution:** Ensure PowerShell is available and you have write permissions

---

## Notes

- **Man pages are optional** - The distribution will build without them, but they're recommended for completeness
- **Regenerate after code changes** - If you modify SU programs, regenerate man pages to reflect changes
- **Distribution is clean** - The script removes any existing distribution directory before creating a new one
- **Sample data** - Only `.su` files from tutorial/examples directories are included

---

## File Locations Reference

| Script/File | Location |
|-------------|----------|
| Man page generator | `zau/tools/generate_man_pages.py` |
| Man page batch wrapper | `zau/tools/run_man_generator.bat` |
| Distribution builder | `zau/scripts/create_distribution.bat` |
| Generated man pages | `man/man1/*.1` |
| Generated markdown | `zau/doc/suman/*.md` |
| Distribution output | `dist/cwpsu-windows-x64/` |
| Distribution ZIP | `dist/cwpsu-windows-x64.zip` |

---

*Last Updated: December 16, 2025*
