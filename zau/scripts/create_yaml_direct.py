#!/usr/bin/env python3
"""Direct YAML creation without yaml library"""

import os
from pathlib import Path

def main():
    try:
        project_root = Path(__file__).parent.parent.parent
        su_main_dir = project_root / "src" / "su" / "main"
        markdown_dir = project_root / "zau" / "doc" / "suman"
        bin_dir = project_root / "bin"
        output_file = project_root / "zau" / "src" / "processtree" / "su_process_tree.yaml"
        
        print(f"Project root: {project_root}")
        print(f"SU main exists: {su_main_dir.exists()}")
        print(f"Markdown dir exists: {markdown_dir.exists()}")
        print(f"Bin dir exists: {bin_dir.exists()}")
        print(f"Output will be: {output_file}")
        
        # Get all .exe files from bin directory (or dist if bin doesn't exist)
        exe_files = set()
        if bin_dir.exists():
            for exe_file in bin_dir.glob("*.exe"):
                exe_name = exe_file.stem  # Remove .exe extension
                exe_files.add(exe_name)
        
        # Also check dist directory if bin is empty
        if len(exe_files) == 0:
            dist_bin = project_root / "dist" / "cwpsu-windows-x64" / "bin"
            if dist_bin.exists():
                for exe_file in dist_bin.glob("*.exe"):
                    exe_name = exe_file.stem
                    exe_files.add(exe_name)
        
        print(f"Found {len(exe_files)} executables")
        if len(exe_files) > 0:
            sample_exes = sorted(list(exe_files))[:5]
            print(f"Sample executables: {', '.join(sample_exes)}")
    except Exception as e:
        print(f"Error in setup: {e}")
        import traceback
        traceback.print_exc()
        return
    
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
    
    try:
        output_file.parent.mkdir(parents=True, exist_ok=True)
        print(f"Output directory ready: {output_file.parent}")
    except Exception as e:
        print(f"Error creating directory: {e}")
        import traceback
        traceback.print_exc()
        return
    
    try:
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("processes:\n")
            
            for item in sorted(os.listdir(su_main_dir)):
                item_path = su_main_dir / item
                if not item_path.is_dir() or item.startswith('.'):
                    continue
                    
                if item in category_map:
                    category_name = category_map[item]
                else:
                    category_name = item.replace('_', ' ').title()
                
                # Get .c files
                c_files = [f for f in os.listdir(item_path) 
                          if f.endswith('.c') and not f.endswith('.c.hold')]
                
                if not c_files:
                    continue
                
                f.write(f"  - category: \"{category_name}\"\n")
                f.write(f"    directory: \"{item}\"\n")
                f.write("    processes:\n")
                
                for c_file in sorted(c_files):
                    process_name = c_file[:-2]  # Remove .c
                    
                    # Only include if there's a corresponding .exe in bin directory
                    if process_name not in exe_files:
                        continue
                    
                    md_file = markdown_dir / f"{process_name}.md"
                    f.write(f"      - name: \"{process_name}\"\n")
                    if md_file.exists():
                        f.write(f"        help_file: \"{process_name}.md\"\n")
                    else:
                        f.write("        help_file: null\n")
        
        print(f"Successfully created: {output_file}")
        print(f"File size: {output_file.stat().st_size} bytes")
    except Exception as e:
        print(f"Error writing file: {e}")
        import traceback
        traceback.print_exc()
        return

if __name__ == "__main__":
    main()
