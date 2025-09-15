# CS3388B Assignment 5
## Western University 2025

## Overview
This project implements the marching cubes algorithm to extract and render triangle meshes from scalar fields in 3D.
The user can explore the surface using a camera implemented via spherical coords, and the mesh is shaded using Phong lighting. The mesh can also be saved to a PLY file.

---

## What I Did and Why

### 1. Marching Cubes Algorithm
Implemented a progressive marching cubes algorithm that:
- Iterates over a scalar field to compute the isosurface based on a chosen isovalue.
- Uses the lookup table (`marching_cubes_lut`) to generate triangles within a 3D grid.
- **Why?** Progressive rendering (100 cubes per frame) creates a growing mesh effect and helps visualize the algorithm in action (we love BONUSes)

### 2. Normals and Mesh Export
- compute_normals() computes face normals for each triangle.
- writePLY() writes vertices and normals to a valid ASCII PLY file, with unshared vertices/face.
- **Why?** This lets exported meshes be visualized w/ lighting in external tools while preserving sharp edges.

### 3. Phong Shading with GLSL
- Implemented a vertex and fragment shader.
- Used uniforms MVP, V, LightDir, and modelColor.
- The fragment shader includes:
  - Ambient term (0.2, 0.2, 0.2)
  - Diffuse and specular (Phong) lighting with shininess 64.
- **Why?** To provide realistic rendering and help distinguish geometry w/ lighting.

### 4. Camera with Spherical Coordinates
- Camera orbits around the origin.
- Mouse dragging updates these angles; up/down keys zoom the camera in/out.
- **Why?** This simulates a virtual trackball, allowing inspection of the mesh from any direction.

### 5. Bounding Box
- A wireframe cube is drawn around the scalar field domain from (min, min, min) to (max, max, max).
- **Why?** Helps the viewer understand the 3D volume being processed.

---

## Known Bugs and Issues
- One issue with camera positioning being too close, workaround made.

---

## Compilation and Running Instructions

### Dependencies
Ensure you have the following libraries installed:
- GLFW
- GLEW

NOTE: you also need glm, but this project was designed not with the library installed on the machine, but
with it directly within the project folder. This decision was made simply due to the fact that it was easy and fool-proof.

When trying to run, you may notice a message saying that a certain directory does not exist. This is an easy fix.
Simply take the line, likely something along the lines of <glm/gtx/dual_quarternon> and change all instances of <> with "".
If it still does not run, attempt "../gtx/dual_quarternon". This should work!

### Controls
- **Click + Drag:** Rotate the camera around the object
- **Up/Down Arrow Keys:** Zoom in/out
- **S Key:** Save current mesh to output.ply

---

## Submitted Files
If there is a & that means that filename has both .cpp and .hpp files
- main.cpp: Program entry point
- Camera.&: Implements spherical coordinate camera
- Renderer.&: Handles rendering the box and mesh using VAO/VBO
- ProgressiveMarcher.&: Marching cubes algorithm with progressive stepping
- Processing.&: Computes normals and exports to output.ply
- ShaderUtils.&: Loads and compiles GLSL shaders
- phong.vert: Vertex shader for lighting setup
- phong.frag`: Fragment shader for Phong shading
- README.md: This documentation file
- one.ply: Sample output mesh (wave)
- two.ply: Sample output mesh (quadratic)
- one.png: Screenshot of mesh (wave)
- two.png: Screenshot of mesh (quadractic)

NOTE: the output files usually are named output.ply, I just renamed them for ease of access post-production.
---

## Author
Created by Beril Coban, March 2025.
