#ifndef PROGRESSIVEMARCHER_HPP
#define PROGRESSIVEMARCHER_HPP

#include <vector>
#include <functional>
#include "glm/glm.hpp"

class ProgressiveMarcher {
public:
    // constructor
    ProgressiveMarcher(std::function<float(float, float, float)> scalarField, float isovalue, float min, float max, float stepSize);

    // provesses one cube in the grid, returns false when all cubes have been processed
    bool step();

    // returns 3D vertex positions
    const std::vector<float>& getVertices() const;

    // returns normal vectors
    const std::vector<float>& getNormals();

private:
    // scalar function field
    std::function<float(float, float, float)> f;

    // paramteres defining grid and surface
    float iso, minVal, maxVal, stepSize;
    int gridSize;   // # of cubes along one axis

    // current marching cube position
    int x, y, z;

    // generated geometry
    std::vector<float> vertices;
    std::vector<float> normals;
    bool normalsDirty;  // dooes normal need to be recomputed

    // processes one cube @ some position in teh grid and appends trianges
    void processCube(int x, int y, int z);
};

#endif
