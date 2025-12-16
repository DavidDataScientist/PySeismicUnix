#!/usr/bin/env python3
"""
Generate YAML process tree from src/su/main directory structure
Maps .c files to markdown help files in zau/doc/suman/
"""

import os
from pathlib import Path

try:
    import yaml
    HAS_YAML = True
except ImportError:
    HAS_YAML = False

def get_project_root():
    """Find project root"""
    current = Path(__file__).resolve().parent.parent.parent
    return current

def get_c_files(directory):
    """Get all .c files in a directory, excluding RCS and other subdirs"""
    c_files = []
    for item in os.listdir(directory):
        if item.endswith('.c') and not item.endswith('.c.hold'):
            c_files.append(item)
    return sorted(c_files)

def process_name_from_c_file(c_file):
    """Extract process name from .c filename"""
    # Remove .c extension
    name = c_file[:-2]
    return name

def check_markdown_exists(process_name, markdown_dir):
    """Check if markdown file exists for process"""
    md_file = markdown_dir / f"{process_name}.md"
    return md_file.exists()

def scan_directory(category_dir, category_name, markdown_dir):
    """Scan a category directory and return process list"""
    processes = []
    c_files = get_c_files(category_dir)
    
    for c_file in c_files:
        process_name = process_name_from_c_file(c_file)
        md_exists = check_markdown_exists(process_name, markdown_dir)
        
        process_entry = {
            "name": process_name,
            "help_file": f"{process_name}.md" if md_exists else None
        }
        processes.append(process_entry)
    
    return processes

def main():
    project_root = get_project_root()
    su_main_dir = project_root / "src" / "su" / "main"
    markdown_dir = project_root / "zau" / "doc" / "suman"
    output_file = project_root / "zau" / "src" / "processtree" / "su_process_tree.yaml"
    
    print(f"Project root: {project_root}")
    print(f"SU main dir: {su_main_dir} (exists: {su_main_dir.exists()})")
    print(f"Markdown dir: {markdown_dir} (exists: {markdown_dir.exists()})")
    print(f"Output file: {output_file}")
    
    # Category mapping - directory name to display name
    category_map = {
        "amplitudes": "Amplitudes",
        "attributes_parameter_estimation": "Attributes & Parameter Estimation",
        "convolution_correlation": "Convolution & Correlation",
        "data_compression": "Data Compression",
        "data_conversion": "Data Conversion",
        "datuming": "Datuming",
        "decon_shaping": "Deconvolution & Shaping",
        "dip_moveout": "Dip Moveout (DMO)",
        "filters": "Filters",
        "gocad": "GOCAD",
        "headers": "Headers",
        "interp_extrap": "Interpolation & Extrapolation",
        "migration_inversion": "Migration & Inversion",
        "multicomponent": "Multicomponent",
        "noise": "Noise",
        "operations": "Operations",
        "picking": "Picking",
        "stacking": "Stacking",
        "statics": "Statics",
        "stretching_moveout_resamp": "Stretching, Moveout & Resampling",
        "supromax": "Supromax",
        "synthetics_waveforms_testpatterns": "Synthetics, Waveforms & Test Patterns",
        "tapering": "Tapering",
        "transforms": "Transforms",
        "velocity_analysis": "Velocity Analysis",
        "well_logs": "Well Logs",
        "windowing_sorting_muting": "Windowing, Sorting & Muting"
    }
    
    categories = []
    
    # Get all directories in src/su/main
    for item in sorted(os.listdir(su_main_dir)):
        item_path = su_main_dir / item
        if item_path.is_dir() and not item.startswith('.'):
            if item in category_map:
                category_name = category_map[item]
            else:
                # Use directory name as category name if not in map
                category_name = item.replace('_', ' ').title()
            
            processes = scan_directory(item_path, category_name, markdown_dir)
            
            if processes:  # Only add if there are processes
                category_entry = {
                    "category": category_name,
                    "directory": item,
                    "processes": processes
                }
                categories.append(category_entry)
    
    # Write YAML file
    output_file.parent.mkdir(parents=True, exist_ok=True)
    
    if HAS_YAML:
        # Use yaml library if available
        yaml_data = {
            "processes": categories
        }
        with open(output_file, 'w', encoding='utf-8') as f:
            yaml.dump(yaml_data, f, default_flow_style=False, sort_keys=False, allow_unicode=True, indent=2)
    else:
        # Write YAML manually
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("processes:\n")
            for cat in categories:
                f.write(f"  - category: \"{cat['category']}\"\n")
                f.write(f"    directory: \"{cat['directory']}\"\n")
                f.write("    processes:\n")
                for proc in cat['processes']:
                    f.write(f"      - name: \"{proc['name']}\"\n")
                    if proc['help_file']:
                        f.write(f"        help_file: \"{proc['help_file']}\"\n")
                    else:
                        f.write("        help_file: null\n")
    
    total_processes = sum(len(cat['processes']) for cat in categories)
    print(f"Generated YAML file: {output_file}")
    print(f"Total categories: {len(categories)}")
    print(f"Total processes: {total_processes}")

if __name__ == "__main__":
    main()
