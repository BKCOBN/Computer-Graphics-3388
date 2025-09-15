#version 330 core

// inputs

layout(location = 0) in vec3 inPosition;    // vertex pos
layout(location = 1) in vec3 inNormal;      // vertex normal

uniform mat4 MVP;       // MVP matrix
uniform mat4 V;         // view matrix
uniform vec3 LightDir;  // direction to light

out vec3 FragNormal;    // transformed + normalized normal vector
out vec3 LightDirection;// normalized direction to light
out vec3 ViewDirection; // direction from frag to camera

void main() {
    // transform pos to view space
    vec4 worldPos = V * vec4(inPosition, 1.0);

    // transform normal to view space
    vec3 transformedNormal = mat3(V) * inNormal;

    // output interpolated values
    FragNormal = normalize(transformedNormal);  // normal
    LightDirection = normalize(LightDir);       // light direction
    ViewDirection = normalize(-worldPos.xyz);   // direction to cam

    // final vertex pos
    gl_Position = MVP * vec4(inPosition, 1.0);
}
