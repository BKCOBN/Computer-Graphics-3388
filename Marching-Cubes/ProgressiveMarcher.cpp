#include "ProgressiveMarcher.hpp"
#include "TriTable.hpp"
#include "glm/glm.hpp"
#include <cmath>

// external lookup table defined in tritable.hpp
extern int marching_cubes_lut[256][16];

// interpoloate a vertex along the edge btwn p1 p2
static glm::vec3 interpolate(float iso, const glm::vec3& p1, const glm::vec3& p2, float val1, float val2) {
    if (fabs(val2 - val1) < 1e-6f) return p1;   // prevent division by zero
    float t = (iso - val1) / (val2 - val1);     // interpolation factor
    return p1 + t * (p2 - p1);                  // interpolated position
}

// initialize marching cubes grid and iteration
ProgressiveMarcher::ProgressiveMarcher(
    // implicit furface function, threshhold where surface is defined, min bound, max bound, step btwn grid points)
    std::function<float(float, float, float)> scalarField, float isovalue, float min, float max, float stepSize)
    : f(scalarField), iso(isovalue), minVal(min), maxVal(max), stepSize(stepSize), x(0), y(0), z(0), normalsDirty(true)
{
    gridSize = static_cast<int>((max - min) / stepSize);    // # cubes along each axis
}

// step function that processes the next in the grid, false when done
bool ProgressiveMarcher::step() {
    if (z >= gridSize) return false;    // all cubes done

    processCube(x, y, z);   // process current cube
    normalsDirty = true;    // mark normals as outdated

    // -> next cube
    x++;
    if (x >= gridSize) {
        x = 0;
        y++;
        if (y >= gridSize) {
            y = 0;
            z++;
        }
    }

    return true; // there are cubes to go
}

// applies marching cubes to a single cube in the grid at an index
void ProgressiveMarcher::processCube(int xi, int yi, int zi) {
    glm::vec3 grid[8];  // world space pos of corners
    float val[8];       // scalar field values @ corners

    // standard corner offsets
    static const glm::ivec3 cornerOffsets[8] = {
        {0, 0, 0}, // 0
        {1, 0, 0}, // 1
        {1, 1, 0}, // 2
        {0, 1, 0}, // 3
        {0, 0, 1}, // 4
        {1, 0, 1}, // 5
        {1, 1, 1}, // 6
        {0, 1, 1}  // 7
    };

    // compute corner positions + scalar values
    for (int i = 0; i < 8; ++i) {
        float px = minVal + (xi + cornerOffsets[i].x) * stepSize;
        float py = minVal + (yi + cornerOffsets[i].y) * stepSize;
        float pz = minVal + (zi + cornerOffsets[i].z) * stepSize;

        grid[i] = glm::vec3(px, py, pz);
        val[i] = f(px, py, pz); // evaluate scalar field
    }

    // determine config using scalar signs
    int cubeIndex = 0;
    for (int i = 0; i < 8; ++i)
        if (val[i] < iso) cubeIndex |= (1 << i);

    // if there are no triangles skip
    if (marching_cubes_lut[cubeIndex][0] == -1) return;

    // define which corners are connected by each edge
    glm::vec3 edgeVertices[12];
    static const int edgeA[12] = {
        0, 1, 2, 3,  // bottom edges
        4, 5, 6, 7,  // top edges
        0, 1, 2, 3   // vertical edges
    };

    static const int edgeB[12] = {
        1, 2, 3, 0,  // bottom edges
        5, 6, 7, 4,  // top edges
        4, 5, 6, 7   // vertical edges
    };

    // compute interpolated positions for intersected edges
    for (int i = 0; i < 12; ++i) {
        edgeVertices[i] = interpolate(iso, grid[edgeA[i]], grid[edgeB[i]], val[edgeA[i]], val[edgeB[i]]);
    }

    // for each triangle listed in the LUT for this congif
    for (int i = 0; marching_cubes_lut[cubeIndex][i] != -1; i += 3) {
        for (int j = 0; j < 3; ++j) {
            glm::vec3 v = edgeVertices[marching_cubes_lut[cubeIndex][i + j]];
            vertices.push_back(v.x);
            vertices.push_back(v.y);
            vertices.push_back(v.z);
        }
    }
}

// current list of vertex positions
const std::vector<float>& ProgressiveMarcher::getVertices() const {
    return vertices;
}

// computes normals and returns them
const std::vector<float>& ProgressiveMarcher::getNormals() {
    if (!normalsDirty) return normals;  // up to date

    // rebuild
    normals.clear();

    // for each triangle
    for (size_t i = 0; i < vertices.size(); i += 9) {
        glm::vec3 p0(vertices[i],     vertices[i+1], vertices[i+2]);
        glm::vec3 p1(vertices[i+3],   vertices[i+4], vertices[i+5]);
        glm::vec3 p2(vertices[i+6],   vertices[i+7], vertices[i+8]);

        // normal
        glm::vec3 n = glm::normalize(glm::cross(p2 - p0, p1 - p0));

        // repeat same normal for all vertis
        for (int j = 0; j < 3; ++j) {
            normals.push_back(n.x);
            normals.push_back(n.y);
            normals.push_back(n.z);
        }
    }

    normalsDirty = false;
    return normals;
}
