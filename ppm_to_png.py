#!/usr/bin/env python3
"""
PPM to PNG Converter
Converts PPM P3 format files to PNG for easier viewing
"""

import sys
import numpy as np
from PIL import Image

def load_ppm_p3(filename):
    """Load a PPM P3 format file and return width, height, and image data"""
    with open(filename, 'r') as f:
        header = f.readline().strip()
        if header != 'P3':
            print(f"Error: {filename} is not a PPM P3 file!")
            return None, None, None

        # Skip comments
        while True:
            line = f.readline().strip()
            if line.startswith('#'):
                continue
            else:
                width, height = map(int, line.split())
                break

        max_color_value = int(f.readline().strip())
        if max_color_value != 255:
            print(f"Warning: Max color value is {max_color_value}, expected 255")

        # Read pixel data
        pixel_data = []
        for line in f:
            pixel_data.extend(map(int, line.split()))

        # Convert pixel data to numpy array and reshape it
        try:
            image = np.array(pixel_data, dtype=np.uint8).reshape((height, width, 3))
        except Exception as e:
            print(f"Error reshaping image data: {e}")
            return None, None, None

    return width, height, image

def convert_ppm_to_png(ppm_file, png_file=None):
    """Convert PPM file to PNG format"""
    if png_file is None:
        png_file = ppm_file.replace('.ppm', '.png')
    
    print(f"Converting {ppm_file} to {png_file}...")
    
    width, height, image = load_ppm_p3(ppm_file)
    if image is None:
        return False
    
    # Convert numpy array to PIL Image
    pil_image = Image.fromarray(image, 'RGB')
    
    # Save as PNG
    pil_image.save(png_file)
    print(f"Successfully converted to {png_file}")
    print(f"Image dimensions: {width}x{height}")
    
    return True

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 ppm_to_png.py <ppm_file> [png_file]")
        print("Example: python3 ppm_to_png.py output.ppm")
        print("Example: python3 ppm_to_png.py output.ppm my_image.png")
        return 1
    
    ppm_file = sys.argv[1]
    png_file = sys.argv[2] if len(sys.argv) > 2 else None
    
    if not ppm_file.endswith('.ppm'):
        print("Warning: Input file doesn't have .ppm extension")
    
    success = convert_ppm_to_png(ppm_file, png_file)
    return 0 if success else 1

if __name__ == "__main__":
    sys.exit(main())