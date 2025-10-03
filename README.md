# Ray Tracing Renderer in C

A 3D ray tracer implemented from scratch in C that generates realistic images of spheres with lighting, shadows, and anti-aliasing. Features automatic image conversion and viewing tools for easy visualization.

## Quick Start

```bash
# Build and test with sample scene
make test

# View the generated image
make view

# Create custom scenes
make run INPUT=your_scene.txt OUTPUT=my_render.ppm
```

## Features

- **Realistic Rendering**: Ray tracing with diffuse lighting and shadow casting
- **Anti-Aliasing**: 3×3 supersampling for smooth edges
- **Easy Viewing**: Automatic PPM to PNG conversion and image viewing
- **Batch Processing**: Render multiple scenes at once
- **Clean Architecture**: Modular C code with proper memory management

## Usage

### Basic Rendering
```bash
# Generate image from scene file
./FS_assg input.txt output.ppm

# Convert to PNG and view
python3 view_image.py output.ppm
```

### Using Makefile Targets
```bash
make test                    # Build, render sample, convert to PNG
make view                    # View default output.ppm
make run INPUT=scene.txt OUTPUT=render.ppm  # Custom render
make view-custom FILE=render.ppm            # View any PPM file
```

### Batch Processing
```bash
python3 batch_render.py "*.txt"  # Render all .txt files
```

## Input Format

Create scene files with this format:
```
<width> <height>           # Image dimensions
<viewport_height>          # Camera viewport height
<focal_length>            # Camera focal length
<light_x> <light_y> <light_z> <brightness>  # Light source
<num_colors>              # Number of colors
<color1_hex> <color2_hex> ...  # Color palette
<background_color_index>  # Background color index
<num_spheres>             # Number of spheres
<sphere_x> <sphere_y> <sphere_z> <radius> <color_index>  # Sphere data
...
```

**Example scene** (`sample_input.txt`):
```
640 480
2.0
1.0
20.0 20.0 10.0 1000.0
4
0x1188EE 0xDD2266 0xDD7700 0x11CC66
0
3
2.0 0.0 -5.0 2.0 2
-2.0 0.0 -5.0 2.0 3
0.0 -102.0 -5.0 100.0 1
```

## Example Output

<div style="text-align: center;">
    <img src="assets/img1.png" alt="Ray traced spheres with lighting and shadows" width="400">
</div>

## Project Structure

- **`src/main.c`** - Main rendering loop and ray casting
- **`src/vector.c`** - 3D vector mathematics
- **`src/spheres.c`** - Sphere intersection and world management
- **`src/color.c`** - Color conversion and PPM output
- **`ppm_to_png.py`** - PPM to PNG converter
- **`view_image.py`** - Automatic image viewer
- **`batch_render.py`** - Batch processing script

## Dependencies

- **C Compiler**: GCC or Clang
- **Python 3**: For image conversion and viewing tools
- **PIL/Pillow**: Python image processing library
- **Make**: Build system

## Documentation

See [VIEWING_GUIDE.md](VIEWING_GUIDE.md) for detailed usage instructions and troubleshooting.
