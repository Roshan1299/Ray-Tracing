#!/usr/bin/env python3
"""
Batch Renderer for Ray Tracing
Processes multiple input files and generates corresponding images
"""

import sys
import os
import subprocess
import glob

def render_scene(input_file, output_file=None):
    """Render a single scene"""
    if output_file is None:
        output_file = input_file.replace('.txt', '.ppm')
    
    print(f"Rendering {input_file} -> {output_file}")
    
    # Run the ray tracer
    result = subprocess.run(['./FS_assg', input_file, output_file], 
                          capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"Error rendering {input_file}: {result.stderr}")
        return False
    
    # Convert to PNG
    png_file = output_file.replace('.ppm', '.png')
    result = subprocess.run(['python3', 'ppm_to_png.py', output_file, png_file],
                          capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"Error converting {output_file}: {result.stderr}")
        return False
    
    print(f"Successfully created {png_file}")
    return True

def batch_render(pattern="*.txt"):
    """Render all files matching the pattern"""
    input_files = glob.glob(pattern)
    
    if not input_files:
        print(f"No files found matching pattern: {pattern}")
        return False
    
    print(f"Found {len(input_files)} input files:")
    for file in input_files:
        print(f"  - {file}")
    
    success_count = 0
    for input_file in input_files:
        if render_scene(input_file):
            success_count += 1
        print()  # Add spacing between renders
    
    print(f"Successfully rendered {success_count}/{len(input_files)} scenes")
    return success_count == len(input_files)

def main():
    if len(sys.argv) > 1:
        pattern = sys.argv[1]
    else:
        pattern = "*.txt"
    
    print(f"Batch rendering files matching: {pattern}")
    success = batch_render(pattern)
    return 0 if success else 1

if __name__ == "__main__":
    sys.exit(main())