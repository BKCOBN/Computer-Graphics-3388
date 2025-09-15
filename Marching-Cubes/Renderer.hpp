#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include "glm/glm.hpp"

class Renderer {
public:
    // initializes OpenGL buffers and sets up bounding box geomtery
    Renderer(float min, float max);

    // cleans OpenGL resources
    ~Renderer();

    // upload vertex and normal data
    void uploadMesh(const std::vector<float>& vertices, const std::vector<float>& normals);

    // renders bounding box and the mesh
    void draw(const glm::mat4& MVP, const glm::mat4& viewMatrix);

private:
    // VAO and VBOs for mesh
    unsigned int vaoMesh, vboVertices, vboNormals;

    // VAO and VBOs for the box
    unsigned int vaoBox, vboBox;

    // # of vertis in mesh
    size_t meshVertexCount = 0;

    // initializes geometry for the box
    void setupBox(float min, float max);
};

#endif
