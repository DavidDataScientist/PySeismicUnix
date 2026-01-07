#!/usr/bin/env python3
"""
Generate man pages from CWP/SU executables
Extracts help text from each program and creates Unix man pages and Markdown docs
"""

import os
import sys
import subprocess
import re
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Configuration
SCRIPT_DIR = Path(__file__).parent.resolve()

def get_bin_path() -> Path:
    """Get absolute path to bin directory"""
    # Try current directory first
    current_dir = Path.cwd()
    if (current_dir / "bin").exists():
        return (current_dir / "bin").resolve()
    
    # Try relative to script (zau/tools -> project root)
    # Script is at: project/zau/tools/generate_man_pages.py
    # Bin is at: project/bin
    # Project root is: project (2 levels up from tools)
    project_root = SCRIPT_DIR.parent.parent
    bin_path = project_root / "bin"
    
    if bin_path.exists():
        return bin_path.resolve()
    
    # Try dist directory (for distribution package)
    dist_bin = project_root / "dist" / "cwpsu-windows-x64" / "bin"
    if dist_bin.exists():
        return dist_bin.resolve()
    
    raise FileNotFoundError(f"Cannot find bin directory. Tried: {current_dir / 'bin'}, {bin_path}, {dist_bin}")

def run_program_help(program_path: Path) -> Tuple[str, bool]:
    """
    Run program to get help text using --help flag
    Returns: (output_text, success)
    success is True ONLY if:
    - Exit code is 0 (success)
    - We have non-empty help output (minimum 10 chars)
    Any error (exit code > 0) means no man page should be created
    """
    try:
        # Use --help flag (Windows port enhancement - exits with code 0 on success)
        result = subprocess.run(
            [str(program_path), "--help"],
            capture_output=True,
            text=True,
            timeout=5,
            cwd=program_path.parent
        )
        output = result.stdout + result.stderr
        output_stripped = output.strip()
        
        # Only return success if exit code is 0 AND we have actual output
        # Exit code 0 = success, anything else = error (don't create man page)
        if result.returncode == 0 and len(output_stripped) >= 10:
            return (output, True)
        
        # If --help returned non-zero, don't try -h (it's an error)
        if result.returncode != 0:
            return (output, False)
        
        # If --help returned 0 but no output, try -h as fallback
        if len(output_stripped) == 0:
            result = subprocess.run(
                [str(program_path), "-h"],
                capture_output=True,
                text=True,
                timeout=5,
                cwd=program_path.parent
            )
            output = result.stdout + result.stderr
            output_stripped = output.strip()
            # Only accept if exit code is 0 and we have output
            if result.returncode == 0 and len(output_stripped) >= 10:
                return (output, True)
        
        # No valid help output found (exit code was 0 but no output, or other error)
        return (output, False)
    except subprocess.TimeoutExpired:
        return ("", False)
    except Exception as e:
        return (f"Error: {str(e)}", False)

def parse_help_text(program_name: str, help_text: str) -> Dict[str, str]:
    """
    Parse SU help text into structured sections
    SU programs typically have:
    - Description (first line or after program name)
    - Usage line
    - Required parameters
    - Optional parameters
    - Examples
    """
    sections = {
        "name": program_name,
        "description": "",
        "synopsis": "",
        "required": "",
        "optional": "",
        "examples": "",
        "notes": "",
        "raw": help_text
    }
    
    lines = help_text.split('\n')
    if not lines:
        return sections
    
    # Extract description (usually first non-empty line or after program name)
    desc_lines = []
    in_desc = False
    for i, line in enumerate(lines):
        line_stripped = line.strip()
        if not line_stripped:
            continue
        
        # Skip program name line
        if line_stripped.upper() == program_name.upper() or line_stripped.startswith(program_name.upper() + ":"):
            in_desc = True
            continue
        
        # Description usually comes before "Required" or "Optional"
        if "required" in line_stripped.lower() or "optional" in line_stripped.lower():
            break
        
        if in_desc or (i < 10 and not any(keyword in line_stripped.lower() for keyword in ["usage", "stdin", "stdout", "parameters"])):
            desc_lines.append(line_stripped)
            in_desc = True
    
    sections["description"] = " ".join(desc_lines[:3])  # First few lines
    
    # Extract synopsis (look for usage patterns)
    for line in lines:
        if "stdin" in line.lower() or "stdout" in line.lower() or "<" in line or ">" in line:
            sections["synopsis"] = line.strip()
            break
    
    # Extract required/optional parameters and other sections
    in_section = None
    current_section = []
    
    for line in lines:
        line_stripped = line.strip()
        line_lower = line_stripped.lower()
        
        # Check for section headers
        if "required parameters" in line_lower and len(line_stripped) < 50:
            if in_section:
                sections[in_section] = "\n".join(current_section).strip()
            in_section = "required"
            current_section = []
            continue
        elif "optional parameters" in line_lower and len(line_stripped) < 50:
            if in_section:
                sections[in_section] = "\n".join(current_section).strip()
            in_section = "optional"
            current_section = []
            continue
        elif ("examples" in line_lower or "example:" in line_lower) and len(line_stripped) < 50:
            if in_section:
                sections[in_section] = "\n".join(current_section).strip()
            in_section = "examples"
            current_section = []
            continue
        elif ("notes" in line_lower or "note:" in line_lower) and len(line_stripped) < 50:
            if in_section:
                sections[in_section] = "\n".join(current_section).strip()
            in_section = "notes"
            current_section = []
            continue
        
        # Collect content for current section
        if in_section and line_stripped:
            current_section.append(line_stripped)
    
    # Save last section
    if in_section:
        sections[in_section] = "\n".join(current_section).strip()
    
    return sections

def generate_man_page(program_name: str, sections: Dict[str, str]) -> str:
    """
    Generate Unix man page (groff/troff format)
    """
    man = f""".TH {program_name.upper()} 1 "{sections.get('name', program_name)}" "CWP/SU" "Seismic Unix Manual"
.SH NAME
{program_name} \\- {sections.get('description', 'Seismic Unix processing tool').split('.')[0]}
.SH SYNOPSIS
.B {program_name}
"""
    
    if sections.get('synopsis'):
        synopsis = sections['synopsis'].replace(program_name, f"\\fB{program_name}\\fR")
        man += f"\n{synopsis}\n"
    else:
        man += "\n[options]\n"
    
    if sections.get('description'):
        man += f""".SH DESCRIPTION
{sections['description']}
"""
    
    if sections.get('required'):
        man += f""".SH REQUIRED PARAMETERS
{sections['required']}
"""
    
    if sections.get('optional'):
        man += f""".SH OPTIONAL PARAMETERS
{sections['optional']}
"""
    
    if sections.get('examples'):
        man += f""".SH EXAMPLES
{sections['examples']}
"""
    
    if sections.get('notes'):
        man += f""".SH NOTES
{sections['notes']}
"""
    
    man += f""".SH SEE ALSO
.BR su (1),
.BR segyread (1),
.BR segywrite (1)
.SH AUTHOR
Colorado School of Mines, Center for Wave Phenomena
.SH COPYRIGHT
Copyright (c) Colorado School of Mines
"""
    
    return man

def generate_markdown_page(program_name: str, sections: Dict[str, str]) -> str:
    """
    Generate Markdown documentation page
    """
    md = f"""# {program_name}

{sections.get('description', 'Seismic Unix processing tool')}

## Synopsis

```bash
{program_name} {sections.get('synopsis', '[options]').replace(program_name, '').strip()}
```

"""
    
    if sections.get('required'):
        md += f"""## Required Parameters

{sections['required']}

"""
    
    if sections.get('optional'):
        md += f"""## Optional Parameters

{sections['optional']}

"""
    
    if sections.get('examples'):
        md += f"""## Examples

```
{sections['examples']}
```

"""
    
    if sections.get('notes'):
        md += f"""## Notes

{sections['notes']}

"""
    
    md += f"""## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
"""
    
    return md

def main():
    """Main function"""
    # Check for single program test mode
    test_program = None
    if len(sys.argv) > 1:
        test_program = sys.argv[1]
        print(f"CWP/SU Man Page Generator - Testing: {test_program}")
    else:
        print("CWP/SU Man Page Generator")
    print("=" * 50)
    
    # Get bin directory
    try:
        bin_path = get_bin_path()
    except FileNotFoundError:
        print("ERROR: Cannot find bin directory")
        print("Please run from project root or ensure bin/ exists")
        sys.exit(1)
    
    print(f"Bin directory: {bin_path}")
    
    # Create output directories
    # Man pages: project root/man/man1
    # Markdown: project/zau/doc/suman
    # Script is at: project/zau/tools/generate_man_pages.py
    # Project root is: project (2 levels up from tools)
    # Zau dir is: project/zau (1 level up from tools)
    project_root = SCRIPT_DIR.parent.parent
    zau_dir = SCRIPT_DIR.parent
    man_dir = project_root / "man" / "man1"
    markdown_dir = zau_dir / "doc" / "suman"
    
    man_dir.mkdir(parents=True, exist_ok=True)
    markdown_dir.mkdir(parents=True, exist_ok=True)
    
    print(f"Man pages: {man_dir}")
    print(f"Markdown: {markdown_dir}")
    print()
    
    # Find executables
    if test_program:
        # Test single program
        exe_file = bin_path / f"{test_program}.exe"
        if not exe_file.exists():
            print(f"ERROR: {exe_file} not found")
            sys.exit(1)
        exe_files = [exe_file]
        print(f"Testing: {test_program}")
    else:
        # Find all executables
        exe_files = sorted(bin_path.glob("*.exe"))
        print(f"Found {len(exe_files)} executables")
    print()
    
    success_count = 0
    fail_count = 0
    
    for exe_file in exe_files:
        program_name = exe_file.stem  # Remove .exe
        
        print(f"Processing {program_name}...", end=" ", flush=True)
        
        # Get help text
        help_text, success = run_program_help(exe_file)
        
        # Only create files if we have valid help output
        # Double-check: success must be True AND we must have non-empty text
        if not success or not help_text or len(help_text.strip()) < 10:
            print("SKIP (no help output)")
            fail_count += 1
            continue
        
        # Parse help
        sections = parse_help_text(program_name, help_text)
        
        # Generate man page
        man_content = generate_man_page(program_name, sections)
        man_file = man_dir / f"{program_name}.1"
        man_file.write_text(man_content, encoding='utf-8')
        
        # Generate markdown
        md_content = generate_markdown_page(program_name, sections)
        md_file = markdown_dir / f"{program_name}.md"
        md_file.write_text(md_content, encoding='utf-8')
        
        print("OK")
        success_count += 1
    
    print()
    print("=" * 50)
    print(f"Summary:")
    print(f"  Success: {success_count}")
    print(f"  Failed:  {fail_count}")
    print(f"  Total:   {len(exe_files)}")
    print()
    print(f"Man pages: {man_dir}")
    print(f"Markdown:  {markdown_dir}")

if __name__ == "__main__":
    main()

