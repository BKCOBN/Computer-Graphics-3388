#include "MarchingCubes.hpp"
#include "TriTable.hpp"
#include "glm/glm.hpp"
#include <cmath>

glm::vec3 vertexInterp(float isolevel, const glm::vec3& p1, const glm::vec3& p2, float valp1, float valp2) {
    if (std::abs(isolevel - valp1) < 1e-5f) return p1;
    if (std::abs(isolevel - valp2) < 1e-5f) return p2;
    if (std::abs(valp1 - valp2) < 1e-5f) return p1;
    float mu = (isolevel - valp1) / (valp2 - valp1);
    return p1 + mu * (p2 - p1);
}

std::vector<float> marching_cubes(
    std::function<float(float, float, float)> f,
    float isovalue,
    float min,
    float max,
    float stepsize
) {
    std::vector<float> vertices;
    int gridSize = static_cast<int>((max - min) / stepsize);

    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y) {
            for (int z = 0; z < gridSize; ++z) {
                glm::vec3 grid[8];
                float val[8];

                for (int i = 0; i < 8; ++i) {
                    float dx = (i & 1) ? stepsize : 0.0f;
                    float dy = (i & 2) ? stepsize : 0.0f;
                    float dz = (i & 4) ? stepsize : 0.0f;

                    float px = min + x * stepsize + dx;
                    float py = min + y * stepsize + dy;
                    float pz = min + z * stepsize + dz;

                    grid[i] = glm::vec3(px, py, pz);
                    val[i] = f(px, py, pz);
                }

                int cubeIndex = 0;
                for (int i = 0; i < 8; ++i)
                    if (val[i] < isovalue) cubeIndex |= (1 << i);

                if (TriTable[cubeIndex][0] == -1) continue;

                glm::vec3 vertList[12];
                static const int edgeA[12] = {0,1,2,3,4,5,6,7,0,1,2,3};
                static const int edgeB[12] = {1,2,3,0,5,6,7,4,4,5,6,7};

                for (int i = 0; i < 12; ++i)
                    vertList[i] = vertexInterp(isovalue, grid[edgeA[i]], grid[edgeB[i]], val[edgeA[i]], val[edgeB[i]]);

                for (int i = 0; TriTable[cubeIndex][i] != -1; i += 3)
                    for (int j = 0; j < 3; ++j) {
                        glm::vec3 v = vertList[TriTable[cubeIndex][i + j]];
                        vertices.push_back(v.x);
                        vertices.push_back(v.y);
                        vertices.push_back(v.z);
                    }
            }
        }
    }

    return vertices;
}
