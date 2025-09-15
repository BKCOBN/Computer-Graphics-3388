#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

// reads the contents of a shader file + return as string
std::string readFile(const char* filePath) {
    std::ifstream file(filePath);   // open file
    std::stringstream buffer;       // string stream to store the file contents
    buffer << file.rdbuf();         // contents -> buffer
    return buffer.str();            // stream -> string + return
}

// constructor
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // read shader source code
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    // string to char pointers
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    int success;        // it works! huzzah!
    char infoLog[512];  // just for error messages

    // compile the vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);  // new obj
    glShaderSource(vertex, 1, &vShaderCode, NULL);  // attach source code
    glCompileShader(vertex);    // compile shader

    // check for errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "issue with vertex shader compilation" << infoLog << std::endl;
    }

    // compile the fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);  // new obj
    glShaderSource(fragment, 1, &fShaderCode, NULL);    // attach the sorce code
    glCompileShader(fragment);  // compile the shader

    // check for errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "issue with fragment shader compilation" << infoLog << std::endl;
    }

    // link the shaders -> shader program
    ID = glCreateProgram();         // make a new shader program
    glAttachShader(ID, vertex);     // attach vertex shader
    glAttachShader(ID, fragment);   // attach frag shader
    glLinkProgram(ID);              // link the program

    // again, check for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "issue with shader linking" << infoLog << std::endl;
    }

    // clean up, delete the objs after linking
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// actually activates the shader program
void Shader::use() {
    glUseProgram(ID);
}
