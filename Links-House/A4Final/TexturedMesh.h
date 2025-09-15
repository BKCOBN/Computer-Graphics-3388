#ifndef TEXTUREDMESH_H
#define TEXTUREDMESH_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "VertexData.h"
#include "TriData.h"

class TexturedMesh {
public:
    // openGL obj IDs
    GLuint vao, vbo, ebo, textureID;

    // pointer to the shader program
    Shader* shader;

    // vectors to store vertex and face data
    std::vector<VertexData> vertices;
    std::vector<TriData> faces;

    // constructor
    TexturedMesh(const std::string& plyFile, const std::string& textureFile, Shader* shader);

    // draws the mesh using MVP matrix
    void draw(glm::mat4 MVP);

    // set up mesh buffers and attributes
    void setupMesh();

    // loads the textures
    void loadTexture(const std::string& textureFile);
};

#endif
