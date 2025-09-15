#ifndef SHADER_UTILS_HPP
#define SHADER_UTILS_HPP

#include <string>
#include <GL/glew.h>

// load compile and link a vertex adn frag shader -> shader program
GLuint loadShaders(const std::string& vertexPath, const std::string& fragmentPath);

#endif
