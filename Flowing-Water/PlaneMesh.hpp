#ifndef PLANEMESH_HPP
#define PLANEMESH_HPP

#include <vector>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderUtils.hpp"
#include "BMPReader.hpp"

// handles generating, rendering, and shading mesh
class PlaneMesh {
    std::vector<float> verts;   // vertex positions (x, y, z)
    std::vector<float> normals; // normal vectors 4 lighting
    std::vector<int> indices;   // quad indices (for tessellation)

    GLuint vao, vbo, nbo, ebo;        // OpenGL buffer objects
    GLuint shaderProgram;             // linked shader program ID
    GLuint tex, displacementTex;      // texture IDs (base and heightmap)

    int numVerts;   // total # of vertices
    int numIndices; // total # of indices
    int debugMode = 0; // DEBUGGING MODES: 0 = normal, 1 = normals, 2 = heightmap, 3 = UVs

    float min, max;              // X and Z bounds for mesh
    glm::vec4 modelColor;        // colour

    // generates vertex and index buffers for grid mesh
    void planeMeshQuads(float min, float max, float stepsize) {
        float y = 0.0f;
        for (float x = min; x <= max; x += stepsize) {
            for (float z = min; z <= max; z += stepsize) {
                // add vertex position
                verts.push_back(x);
                verts.push_back(y);
                verts.push_back(z);

                // default normal pointing up
                normals.push_back(0.0f);
                normals.push_back(1.0f);
                normals.push_back(0.0f);
            }
        }

        // calculate indices for quads (stored as 4 control points/patch)
        int nCols = (max - min) / stepsize + 1;
        for (int i = 0; i < nCols - 1; ++i) {
            for (int j = 0; j < nCols - 1; ++j) {
                indices.push_back(i * nCols + j);
                indices.push_back(i * nCols + j + 1);
                indices.push_back((i + 1) * nCols + j + 1);
                indices.push_back((i + 1) * nCols + j);
            }
        }
    }

public:
    // constructor to build mesh, load shaders and textures
    PlaneMesh(float min, float max, float stepsize) {
        this->min = min;
        this->max = max;
        modelColor = glm::vec4(0, 1.0f, 1.0f, 1.0f);    //cyanish

        // generate vertex normal and index data
        planeMeshQuads(min, max, stepsize);
        numVerts = verts.size() / 3;
        numIndices = indices.size();

        // vertex array obj
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // vertex positions (location 0)
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        // normals (loc. 1)
        glGenBuffers(1, &nbo);
        glBindBuffer(GL_ARRAY_BUFFER, nbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);

        // element indices
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

        // shader adn texture setup (+ debugging lines to console)

        std::cout << "[DEBUG] Compiling shaders...\n";
        shaderProgram = LoadShaders("WaterShader.vertexshader", "WaterShader.tcs", "WaterShader.tes", "WaterShader.geoshader", "WaterShader.fragmentshader");
        std::cout << "[DEBUG] Shader program ID: " << shaderProgram << "\n";

        // load base colour texture and height map
        tex = LoadBMPTexture("Assets/water.bmp");
        displacementTex = LoadBMPTexture("Assets/displacement-map1.bmp");
        std::cout << "[DEBUG] Texture IDs - Base: " << tex << ", Displacement: " << displacementTex << "\n";
    }

    // switch debug render mode (to see shader logic visually)
    void setDebugMode(int mode) {
        debugMode = mode;
    }

    // sends uniforms and issues the draw call
    void draw(glm::vec3 lightPos, glm::mat4 V, glm::mat4 P) {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glPatchParameteri(GL_PATCH_VERTICES, 4); // 4-control point patches
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode for debugging

        // compute transformation matrices
        glm::mat4 M = glm::mat4(1.0f);  // model transform = identity centered @ origin
        glm::mat4 MVP = P * V * M;      // final MVP matrix
        float time = glfwGetTime();     // animation time

        // set uniform variables for rendering
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "V"), 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "M"), 1, GL_FALSE, glm::value_ptr(M));
        glUniform3fv(glGetUniformLocation(shaderProgram, "LightPosition_worldspace"), 1, glm::value_ptr(lightPos));

        glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
        glUniform1f(glGetUniformLocation(shaderProgram, "texOffset"), 0.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "texScale"), 40.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "alpha"), 20.0f);               // phong shiniess
        glUniform1f(glGetUniformLocation(shaderProgram, "displacementHeight"), 0.1f);   // water bumpiness
        glUniform1f(glGetUniformLocation(shaderProgram, "displacementCloseness"), 7.0f);    // map frequency
        glUniform1f(glGetUniformLocation(shaderProgram, "innerTess"), 40.0f);               // inside tess density
        glUniform1f(glGetUniformLocation(shaderProgram, "outerTess"), 40.0f);               // outer tess density

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, displacementTex);
        glUniform1i(glGetUniformLocation(shaderProgram, "heightTex"), 1);

        // set mode for debug visualization
        glUniform1i(glGetUniformLocation(shaderProgram, "debugMode"), debugMode);

        // issue draw call (tessellated patches)
        glDrawElements(GL_PATCHES, numIndices, GL_UNSIGNED_INT, 0);
    }
};

#endif
