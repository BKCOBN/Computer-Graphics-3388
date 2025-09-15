// modern opengl
#version 330 core

layout (location = 0) in vec3 aPos;         // input attribute for the vertex position (x, y, x) (0)
layout (location = 1) in vec2 aTexCoord;    // input attribute for texture coords (u, v) (1)

out vec2 TexCoord; // coords from vertex -> fragment

uniform mat4 MVP;   // uniform

// apply the MVP transformation -> vertex position
void main() {
    gl_Position = MVP * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
