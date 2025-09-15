# CS3388B 001, Assignment 4
## Western University 2025

## Overview
This project implements a first-person camera system for exploring a 3D world composed of textured triangle meshes loaded from ply files.
The program parses and renders multiple 3D models using OpenGL, GLSL shaders, and texture mapping, allowing the user to navigate the scene freely.

---

## What I Did and Why

### 1. First-Person Camera Implementation
I implemented a first-person camera that allows movement and rotation using the arrow keys.
- **Movement:** Moves forward and backward in the current direction.
- **Rotation:** Rotates left and right without changing position.

### 2. Mesh Loading from PLY Files
I created a PLY file parser that extracts:
- **Vertex position** (x, y, z)
- **Normal vectors** (nx, ny, nz)
- **Texture coordinates** (u, v)
- **Face indices** (vertex_indices)
- **Why?** This ensures compatibility with PLY models while keeping the parser flexible for files w/ optional attributes.

### 3. Rendering with VAOs, VBOs, and EBOs
Each model is stored in a Vertex Array Object (VAO) and rendered using glDrawElements.
- **Why?** VAOs improve performance since buffer bindings are set once and reused for rendering.

### 4. Texture Mapping
Each PLY file is paired with a .bmp texture, loaded using loadARGB_BMP().
- **Why?** It makes sure that each object has a unique texture w/o needing a separate material system.

---

## Known Bugs and Issues
- **Objects overlap incorrectly sometimes:** This was fixed by enabling GL_DEPTH_TEST.
- **No collision detection:** The camera can move through walls and objects freely (intentional for this assignment).

---

## Compilation and Running Instructions

### Dependencies
Before compiling, make sure you have the following installed:
- **OpenGL** (Automatically included with most graphics drivers)
- **GLFW** (Window management)
- **GLEW** (Handles OpenGL extensions)
- **GLM** (For matrix transformations)
And also make sure you have all of the required bmp and ply files!

### How I Set Up the Camera for the Screenshot
To capture the screenies, I just had to make sure that i:
- Captured as much of the inside of the house as possible
- Captured the outside of the house as well.

---

## Submitted Files:
- `main.cpp`: Entry point for the program
- `Shader.cpp, Shader.h`: Handles shader compilation and linking
- `TexturedMesh.cpp, TexturedMesh.h`: Manages rendering of textured meshes
- `Camera.cpp, Camera.h`: Implements first-person camera controls
- `readPLYFile.cpp, readPLYFile.h`: Loads and parses PLY files
- `LoadBitmap.cpp, LoadBitmap.h`: Loads BMP textures
- `TriData.h`: Stores indices of 3 vertices
- `VertexData.h`: Stores position, normal, and texture coords
- `vertex_shader.glsl`: GLSL vertex shader
- `fragment_shader.glsl`: GLSL fragment shader
- `README.md`: This documentation file

---

## Author:
Created by Beril Coban, March 2025.
