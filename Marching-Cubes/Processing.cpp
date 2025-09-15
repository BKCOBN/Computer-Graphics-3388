#include "Processing.hpp"
#include "glm/glm.hpp"
#include <fstream>
#include <iostream>

// compute normals
// takes a list of verts and returns normals
std::vector<float> compute_normals(const std::vector<float>& vertices) {
    std::vector<float> normals;

    // loop through each triangle (9 floats/triange = 3 verts * 3 coords)
    for (size_t i = 0; i < vertices.size(); i += 9) {
        // extract verts p0 -> p2
        glm::vec3 p0(vertices[i], vertices[i+1], vertices[i+2]);
        glm::vec3 p1(vertices[i+3], vertices[i+4], vertices[i+5]);
        glm::vec3 p2(vertices[i+6], vertices[i+7], vertices[i+8]);

        // compute the normal w/ right hand rules
        // (p1 - p0) x (p2 - p0)
        glm::vec3 n = glm::normalize(glm::cross(p1 - p0, p2 - p0));

        // store the same normal for all 3 triangle vertices
        for (int j = 0; j < 3; ++j) {
            normals.push_back(n.x);
            normals.push_back(n.y);
            normals.push_back(n.z);
        }
    }

    return normals;
}

// function to write the mesh to a PLY file
// each face defined by 3 unique verts
void writePLY(
    const std::vector<float>& vertices, // vertex positions
    const std::vector<float>& normals,  // normals
    const std::string& fileName         // destination file
)

{
    // failure to open the file
    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << fileName << " for writing.\n";
        return;
    }

    size_t numVertices = vertices.size() / 3;   // each vertex has 3 parts
    size_t numFaces = numVertices / 3;          // 3 vertices/tri

    // header
    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << numVertices << "\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "property float nx\n";
    file << "property float ny\n";
    file << "property float nz\n";
    file << "element face " << numFaces << "\n";
    file << "property list uchar int vertex_indices\n";
    file << "end_header\n";

    // write vertices w/ normals (x y z nx ny nz)
    for (size_t i = 0; i < numVertices; ++i) {
        file << vertices[i*3] << " "
             << vertices[i*3 + 1] << " "
             << vertices[i*3 + 2] << " "
             << normals[i*3] << " "
             << normals[i*3 + 1] << " "
             << normals[i*3 + 2] << "\n";
    }

    // write faces (3 idx0 idx1 idx2)
    for (size_t i = 0; i < numFaces; ++i) {
        file << "3 " << (i*3) << " " << (i*3 + 1) << " " << (i*3 + 2) << "\n";
    }

    file.close();
    std::cout << "Mesh written to " << fileName << "\n";
}
