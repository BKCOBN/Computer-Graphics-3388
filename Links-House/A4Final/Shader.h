#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader {
public:
    GLuint ID;  // the shader program ID

    //constructor
    Shader(const char* vertexPath, const char* fragmentPath);
    // activate the shader program
    void use();
};

#endif
