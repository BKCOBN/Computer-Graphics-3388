#include "ShaderUtils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// reads shader and returns contents in a single string
static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Error: cannot open shader file " << path << "\n";
        return "";
    }
    // stringsteam to read it all
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();    // return
}

// load compile and link vert and frag shader -> shader program
GLuint loadShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    // read source code
    std::string vertSrc = readFile(vertexPath);
    std::string fragSrc = readFile(fragmentPath);

    // convert to strings that OpenGL needs
    const char* vShaderCode = vertSrc.c_str();
    const char* fShaderCode = fragSrc.c_str();

    // create and compile the vert amd frag shader objs
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // give source code and compile for vert shader
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);

    // give source code and compile for frag shader
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);

    // link the shaders into shader program
    GLuint program = glCreateProgram();     // empty program
    glAttachShader(program, vertexShader);  // attach vert shader
    glAttachShader(program, fragmentShader);// attach frag shader
    glLinkProgram(program);                 // link

    // delete, they are linked now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // return the compiled and linked shader program ID
    return program;
}
