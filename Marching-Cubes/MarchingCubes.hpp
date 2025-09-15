#ifndef MARCHINGCUBES_HPP
#define MARCHINGCUBES_HPP

#include <functional>
#include <vector>

std::vector<float> marching_cubes(
    std::function<float(float, float, float)> f,
    float isovalue,
    float min,
    float max,
    float stepsize
);

#endif
