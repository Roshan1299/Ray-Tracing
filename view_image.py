#!/usr/bin/env python3
"""
Image Viewer for Ray Tracing Output
Automatically converts PPM to PNG and opens it for viewing
"""

import sys
import os
import subprocess
from ppm_to_png import convert_ppm_to_png

def open_image(image_path):
    """Open image with the default system viewer"""
    try:
        if sys.platform == "darwin":  # macOS
            subprocess.run(["open", image_path])
        elif sys.platform == "linux":
            subprocess.run(["xdg-open", image_path])
        elif sys.platform == "win32":  # Windows
            os.startfile(image_path)
        else:
            print(f"Please open {image_path} with your preferred image viewer")
            return False
        return True
    except Exception as e:
        print(f"Error opening image: {e}")
        return False

def view_ppm_image(ppm_file):
    """Convert PPM to PNG and open it"""
    if not os.path.exists(ppm_file):
        print(f"Error: {ppm_file} does not exist")
        return False
    
    # Convert to PNG
    png_file = ppm_file.replace('.ppm', '.png')
    success = convert_ppm_to_png(ppm_file, png_file)
    
    if not success:
        return False
    
    # Open the image
    print(f"Opening {png_file}...")
    return open_image(png_file)

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 view_image.py <ppm_file>")
        print("Example: python3 view_image.py output.ppm")
        return 1
    
    ppm_file = sys.argv[1]
    success = view_ppm_image(ppm_file)
    return 0 if success else 1

if __name__ == "__main__":
    sys.exit(main())