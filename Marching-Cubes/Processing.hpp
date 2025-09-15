#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <vector>
#include <string>

// computes surface normals
std::vector<float> compute_normals(const std::vector<float>& vertices);

// triangle mesh -> .ply file
void writePLY(const std::vector<float>& vertices, const std::vector<float>& normals, const std::string& fileName);

#endif
