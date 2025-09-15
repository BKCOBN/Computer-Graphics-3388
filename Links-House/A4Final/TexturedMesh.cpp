#include "TexturedMesh.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "LoadBitmap.h"

// function protoype for reading PLY files
extern void readPLYFile(std::string fname, std::vector<VertexData>& vertices, std::vector<TriData>& faces);

// constructor
TexturedMesh::TexturedMesh(const std::string& plyFile, const std::string& textureFile, Shader* shaderProgram) {
    shader = shaderProgram;                 // store reference to shader program
    readPLYFile(plyFile, vertices, faces);  // load vertex and face data from PLY
    setupMesh();                            // set up vertex buffer obj (VBO, VAO, EBO)
    loadTexture(textureFile);               // load texture from bmp
}

// set up the mesh by making buffers and uploading the relevant data
void TexturedMesh::setupMesh() {
    // store the vertex info (pos, texture coords)
    std::vector<float> vertexData;
    // store face indices
    std::vector<unsigned int> indices;

    // process the vertex data
    for (const auto& v : vertices) {
        // push position (x, y, z)
        vertexData.push_back(v.position.x);
        vertexData.push_back(v.position.y);
        vertexData.push_back(v.position.z);

        // push texture coords (u, v)
        vertexData.push_back(v.texCoords.x);
        vertexData.push_back(v.texCoords.y);
    }

    // process the face data
    for (const auto& f : faces) {
        // indec buffer
        indices.push_back(f.indices[0]);
        indices.push_back(f.indices[1]);
        indices.push_back(f.indices[2]);
    }

    // generate the buffers (VAO, VBO, EBO in that order)
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind VAO
    glBindVertexArray(vao);

    // upload the vertex info
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // upload the index info
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // define vertex attributes
    // position (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coords (2 floats)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind VAO
    glBindVertexArray(0);
}

// load bmp texture files + upload it as a texture
void TexturedMesh::loadTexture(const std::string& textureFile) {
    // variables
    unsigned char* imageData;
    unsigned int width, height;

    // load image data from the bmp
    loadARGB_BMP(textureFile.c_str(), &imageData, &width, &height);

    // generate the openGL textures
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // upload the data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set the parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                   // repeat horizontally
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);                   // repeat vert
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // smooth scaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);               // magnification

    glBindTexture(GL_TEXTURE_2D, 0);    // unbind texture
    delete[] imageData;                 // free memory
}

// draw the mesh using the MVP matrix
void TexturedMesh::draw(glm::mat4 MVP) {
    shader->use();  // activat ehte shader program

    // pass matrix to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

    // bind texture to render
    glBindTexture(GL_TEXTURE_2D, textureID);

    // bind VAO and draw the mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);

    // unbind VAO
    glBindVertexArray(0);
}
