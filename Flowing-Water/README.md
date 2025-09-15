# CS3388B Assignment 6: Water Shader  
## Western University 2025

## Overview  
This project implements a real-time water surface renderer using OpenGL 4.5 with tessellation and geometry shaders, Phong shading, and Gerstner wave simulation. A textured plane is deformed dynamically using a displacement map and time-based wave functions. The user can explore the water surface using a spherical camera system with mouse and keyboard controls.

---

## What I Did and Why

### 1. Tessellation Pipeline  
- Implemented all 5 stages: vertex, TCS, TES, geometry, and fragment.
- Tessellation Control Shader (TCS) passes attributes and sets tess levels.
- Tessellation Evaluation Shader (TES) interpolates between patch corners.
- **Why?** To dynamically control surface detail (resolution) without uploading new geometry to the GPU.

### 2. Geometry Shader: Displacement + Gerstner Waves  
- Used a displacement map texture to displace vertices vertically.
- Simulated Gerstner waves with a combo of sin functions (amplitude, direction, frequency, steepness).
- Normals computed per triangle in world space using cross product.
- **Why?** Combining a displacement texture with animated waves creates a realistic, dynamic water surface.

### 3. Fragment Shader: Phong Lighting + Debug Modes  
- Classic Phong lighting using ambient, diffuse, and specular terms.
- Supports multiple debug visualizations:
1. Normals (RGB)
2. Height map (grayscale)
3. UV mapping (checker)
- **Why?** Debug modes allow quick verification of texture coordinates, normals, and height data. Phong lighting gives a realistic result with specular highlights.

### 4. GLSL Shader Pipeline and Utilities  
- All shaders compiled and linked using a utility class with verbose error output.
- Full shader program: `WaterShader.vertexshader`, `.tcs`, `.tes`, `.geoshader`, `.fragmentshader`
- **Why?** Separating shader compilation makes it easier to debug and maintain.

### 5. Camera with Spherical Coordinates  
- Camera orbits around the water plane.
- Controlled using mouse drag and up/down arrows for zoom.
- Prevents flipping and clamps vertical angle.
- **Why?** Gives an intuitive and smooth 3D orbiting experience ideal for inspecting curved wave surfaces.

### 6. PlaneMesh with Tessellated Grid  
- Generated a uniform grid of vertices on the CPU.
- Passed them as quads to the tessellation stage.
- **Why?** Offers a flexible surface that can be subdivided and displaced entirely on the GPU.

---

## Known Bugs and Issues  
- Some spiky visual artifacts can occur if `displacementHeight` is too high or tessellation levels are too low.
- Requires a GPU that supports OpenGL 4.0+ (due to tessellation shaders).

---

## Compilation and Running Instructions

### Dependencies  
Make sure the following libraries are available and linked:
- **GLFW** — for window and input handling  
- **GLEW** — for OpenGL function loading  
- **GLM** — for math (note: included locally in this project)

💡 **Important:**  
GLM is used as local includes (via `"glm/..."`) instead of `<glm/...>`.  
If it fails to compile, change angle brackets to quotes or adjust the relative path like `../glm/...`.

### Controls  
- **Mouse Drag (Left Button):** Rotate camera  
- **Arrow Up / Down:** Zoom in/out  
- **1, 2, 3, 4:** Switch rendering mode (you need to hold the key)  
1 = Normal shading (default)  
2 = Normals (RGB)    
3 = Height Map (grayscale)  
4 = UV Mapping  

---

## Submitted Files  
- `A6-Water.cpp`: Program entry point  
- `PlaneMesh.hpp`: Creates and draws the tessellated grid  
- `GlobeCamera.hpp`: Camera with spherical controls  
- `ShaderUtils.hpp`: Loads, compiles, and links shaders  
- `BMPReader.hpp`: Loads BMP images into OpenGL textures  
- `WaterShader.vertexshader`: transforms input positions to worldspace and computes UVs and directions to eye/light
- `WaterShader.tcs`: passes per-vertex attributes forward, + sets inner and outer tess lvls
- `WaterShader.tes`: interpolates vertext data across the tessellated quad, outputs final vertex world position to geo shader
- `WaterShader.geoshader`: applies height map and Gerstner wave displacement, computer normals per trianges and outputs world position, normal + light/eye vectors
- `WaterShader.fragmentshader`: performs Phong shading + supports debug visualizations
- `MakeFile`: Makefile
- `Assets/water.bmp`: texture map for water surface appearance
- `Assets/displacement-map1.bmp`: height map used to displace verts vertically 
- `README.md`: This file
- screenshots 1, 2, 3: display the waves from differing angles

---

## Author  
Created by **Beril Coban**, April 2025.  