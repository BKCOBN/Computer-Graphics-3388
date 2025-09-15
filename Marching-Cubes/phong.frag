#version 330 core

// inputs from vert shader
in vec3 FragNormal;
in vec3 LightDirection;
in vec3 ViewDirection;

// uniform: base diffuse color
uniform vec3 modelColor;

// output color
out vec4 FragColor;

void main() {
    // normalize all input vectors
    vec3 N = normalize(FragNormal);     // at surface point
    vec3 L = normalize(LightDirection); // direction to light
    vec3 V = normalize(ViewDirection);  // direction to viewer
    vec3 R = reflect(-L, N);            // reflection vector

    //PHONG

    // ambient lighting
    vec3 ambient = vec3(0.2); //low

    // light scattered
    vec3 diffuse = modelColor * max(dot(N, L), 0.0);    // lambertian

    //specular lighting (highlights)
    vec3 specular = vec3(1.0) * pow(max(dot(R, V), 0.0), 64.0);

    // final color
    vec3 color = ambient + diffuse + specular;

    // output it will full opacity
    FragColor = vec4(color, 1.0);
}
