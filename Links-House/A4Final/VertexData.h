#ifndef VERTEXDATA_H
#define VERTEXDATA_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct VertexData {
    // position of vertex, normal vector, color, texture coords
    glm::vec3 position, normal, color;
    glm::vec2 texCoords;

    // default constructor
    VertexData() : position(0.0f), normal(0.0f), color(1.0f), texCoords(0.0f) {}
};

#endif
