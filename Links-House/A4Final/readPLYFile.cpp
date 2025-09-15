#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "VertexData.h"
#include "TriData.h"

// function to read PLY file + parse vertex + face data
void readPLYFile(std::string fname, std::vector<VertexData>& vertices, std::vector<TriData>& faces) {
    // open the file
    std::ifstream file(fname);
    if (!file.is_open()) {
        std::cerr << "File could not open: " << fname << std::endl;
        return;
    }

    // temp variable there to store each line of the file
    std::string line;

    // counters for # of vertices + faces
    int vertexCount = 0, faceCount = 0;

    // flags
    bool headerParsed = false, hasNormals = false, hasTexCoords = false;

    // read header to get vertex and face count (+ check for normals/texture coords)
    while (std::getline(file, line)) {
        if (line.find("element vertex") != std::string::npos) {
            vertexCount = std::stoi(line.substr(15));   // # of verticies
        } else if (line.find("element face") != std::string::npos) {
            faceCount = std::stoi(line.substr(13));     // # of faces
        } else if (line.find("property float nx") != std::string::npos) {
            hasNormals = true;                          // contains normal vectors?
        } else if (line.find("property float u") != std::string::npos) {
            hasTexCoords = true;                        // texture coords
        } else if (line == "end_header") {
            headerParsed = true;                        // end of header section -> exit
            break;
        }
    }

    // read vertex data and add to verticies vector
    for (int i = 0; i < vertexCount; i++) {
        VertexData vertex;  // temporary variable to store vertex data
        std::string line;

        // read the entire line
        if (!std::getline(file, line)) {
            std::cerr << "Error: Unexpected end of file while reading vertex data." << std::endl;
            break;
        }

        std::stringstream ss(line); // create string stream from line
        std::string token;  // hold extracted tokens

        // read x, y, z positions
        if (ss >> token) {
                vertex.position.x = std::stof(token);
        }
        if (ss >> token) {
                vertex.position.y = std::stof(token);
        }
        if (ss >> token) {
                vertex.position.z = std::stof(token);
        }

        // read normal data if available
        if (hasNormals) {
            if (ss >> token) {
                    vertex.normal.x = std::stof(token);
            }
            if (ss >> token) {
                vertex.normal.y = std::stof(token);
            }
            if (ss >> token) {
                    vertex.normal.z = std::stof(token);
            }
        }

        // Read texture coords if available
        if (hasTexCoords) {
            if (ss >> token) {
                vertex.texCoords.x = std::stof(token);
            }
            if (ss >> token) {
                vertex.texCoords.y = std::stof(token);
            }
        }
        // store the parsed vertex into the verticies vector
        vertices.push_back(vertex);
    }


    // read face data and make sure they r triangles (3 verts)
    for (int i = 0; i < faceCount; i++) {
        int vertexCount, v1, v2, v3;    // temp variables to sotre face data
        file >> vertexCount;            // should always be three
        if (vertexCount != 3) {         // so check if it is three or not
            std::cerr << "Non-triangle face detected." << std::endl;    // send message to console
            continue;
        }   // confirmed all triangle faces
        file >> v1 >> v2 >> v3; // read vertex indices for the trianges
        faces.push_back({v1, v2, v3});  // store face data
    }
    // close file
    file.close();
}
