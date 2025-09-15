// ShaderUtils.hpp
#ifndef SHADER_UTILS_HPP
#define SHADER_UTILS_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

// loads contents of file into simple string
// returns empty string if file failed to open
inline std::string LoadFileAsString(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // read file contents into stringstream
    return buffer.str();    // return as a string
}

// compiles a shader, returns shader ID (or 0 if it failed)
inline GLuint CompileShader(const char* path, GLenum shaderType) {
    std::string code = LoadFileAsString(path);  // load from file
    if (code.empty()) return 0;

    const char* src = code.c_str();                 // get C-style string
    GLuint shaderID = glCreateShader(shaderType);   // make shader obj
    glShaderSource(shaderID, 1, &src, nullptr);     // upload source code
    glCompileShader(shaderID);                      // compile shader

    // see if it was successful
    GLint result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (!result) {
        // outlog error log
        char log[1024];
        glGetShaderInfoLog(shaderID, 1024, NULL, log);
        std::cerr << "[ERROR] Failed to compile shader: " << path << "\n" << log << std::endl;
        return 0;
    }
    return shaderID;
}

// loads and links a full shader program, returns final linked program ID or 0 if it failed
inline GLuint LoadShaders(const char* vertexPath, const char* tcsPath, const char* tesPath, const char* geomPath, const char* fragPath) {
    // all 5 shaders
    GLuint vs = CompileShader(vertexPath, GL_VERTEX_SHADER);        // vertex
    GLuint tcs = CompileShader(tcsPath, GL_TESS_CONTROL_SHADER);    // tcs
    GLuint tes = CompileShader(tesPath, GL_TESS_EVALUATION_SHADER); // tes
    GLuint gs = CompileShader(geomPath, GL_GEOMETRY_SHADER);        // geo
    GLuint fs = CompileShader(fragPath, GL_FRAGMENT_SHADER);        // frag

    // if any shader failed to compile
    if (!vs || !tcs || !tes || !gs || !fs) {
        std::cerr << "[ERROR] One or more shaders failed to compile." << std::endl;
        return 0;
    }

    // make a new shader program
    GLuint programID = glCreateProgram();

    // attach all of the compiled shaders
    glAttachShader(programID, vs);
    glAttachShader(programID, tcs);
    glAttachShader(programID, tes);
    glAttachShader(programID, gs);
    glAttachShader(programID, fs);

    // link the complete program
    glLinkProgram(programID);

    // check for any errors with the linking
    GLint linkStatus;
    glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {
        char log[1024];
        glGetProgramInfoLog(programID, 1024, NULL, log);
        std::cerr << "[ERROR] Program link failed:\n" << log << std::endl;
        return 0;
    }

    // if it was successful print to console to relish in your victories
    std::cout << "[INFO] Shader program linked successfully. ID: " << programID << std::endl;

    // post link, delete the individual shaders
    glDeleteShader(vs);
    glDeleteShader(tcs);
    glDeleteShader(tes);
    glDeleteShader(gs);
    glDeleteShader(fs);

    // return the complete linked shader program
    return programID;
}

#endif // SHADER_UTILS_HPP
