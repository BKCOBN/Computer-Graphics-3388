// modern opengl
#version 330 core

out vec4 FragColor; // output variable for the final colour of the fragment
in vec2 TexCoord;   // get the texture coords from the vertex shader

uniform sampler2D texture1; // sampler to get vertex data

void main() {
    vec4 texColor = texture(texture1, TexCoord);    //sample and store

    if (texColor.a < 0.1) discard;  // for transparency

    FragColor = texColor;   // final colour to sampled texture colour
}

