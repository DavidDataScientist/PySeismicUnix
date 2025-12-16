# CWP/SU Tools Scripts

## generate_man_pages.py

Generates Unix-style man pages and Markdown documentation from CWP/SU executables.

### Usage

```batch
cd zau\tools
python generate_man_pages.py
```

Or use the batch wrapper:
```batch
run_man_generator.bat
```

### What It Does

1. Scans `bin\` directory for all `.exe` files
2. Runs each program to extract help text
3. Parses help text into sections (description, synopsis, parameters, examples)
4. Generates:
   - **Unix man pages** in `man/man1/` (project root)
   - **Markdown docs** in `zau/doc/suman/`

### Output

- **Man pages**: `man/man1/{program}.1` (groff/troff format)
- **Markdown**: `zau/doc/suman/{program}.md`

### Requirements

- Python 3.x
- Access to `bin\` directory with SU executables

### Notes

- Programs that don't output help text are skipped
- Parsing handles standard SU help format
- Some programs may need manual review/adjustment

