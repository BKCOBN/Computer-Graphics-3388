#include "Renderer.hpp"
#include <GL/glew.h>
#include "glm/gtc/type_ptr.hpp"
#include <vector>

// initialize VBOs and VAOs
Renderer::Renderer(float min, float max) {
    // allocate and generate VAO VBO for the mesh
    glGenVertexArrays(1, &vaoMesh);
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboNormals);

    // setup the box that bounds the mesh
    setupBox(min, max);
}

// release the OpenGL resources
Renderer::~Renderer() {
    // clean mesh buffers
    glDeleteVertexArrays(1, &vaoMesh);
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboNormals);

    // bounding box buffers
    glDeleteVertexArrays(1, &vaoBox);
    glDeleteBuffers(1, &vboBox);
}

// upload vertex and normal data to GPU buffers for rendering
void Renderer::uploadMesh(const std::vector<float>& vertices, const std::vector<float>& normals) {
    meshVertexCount = vertices.size() / 3;  // three floats a vertex

    glBindVertexArray(vaoMesh);

    // upload the positions
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    // 0 = position
    glEnableVertexAttribArray(0);

    // uplaod vertex normals
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    // 1 = normal
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);   // unbind
}

// draw bounding box + the mesh
void Renderer::draw(const glm::mat4& MVP, const glm::mat4& viewMatrix) {
    // bounding box
    glBindVertexArray(vaoBox);
    glDrawArrays(GL_LINES, 0, 24);  // 12 lines x 2 vertis

    // mesh
    glBindVertexArray(vaoMesh);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(meshVertexCount));
}

// initialize bounding box based on minimum + maximum coords
void Renderer::setupBox(float min, float max) {
    // 8 corner points
    glm::vec3 corners[8] = {
        {min, min, min}, {max, min, min}, {max, max, min}, {min, max, min}, // btm face
        {min, min, max}, {max, min, max}, {max, max, max}, {min, max, max}  // top face
    };

    // 12 edges
    int indices[] = {
        0,1, 1,2, 2,3, 3,0, // bottom
        4,5, 5,6, 6,7, 7,4, // top
        0,4, 1,5, 2,6, 3,7  // sides
    };

    // flatten edge vertis -> float list
    std::vector<float> boxLines;
    for (int i = 0; i < 24; ++i) {
        glm::vec3 v = corners[indices[i]];
        boxLines.push_back(v.x);
        boxLines.push_back(v.y);
        boxLines.push_back(v.z);
    }

    // generate + upload bounding box VBO
    glGenVertexArrays(1, &vaoBox);  // VAO for box
    glGenBuffers(1, &vboBox);       // VBO for line segments

    glBindVertexArray(vaoBox);
    glBindBuffer(GL_ARRAY_BUFFER, vboBox);
    glBufferData(GL_ARRAY_BUFFER, boxLines.size() * sizeof(float), boxLines.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    // 0 = box vertis
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);   // unbind
}
