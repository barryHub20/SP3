#ifndef LOAD_HMAP_H
#define LOAD_HMAP_H
#include "Vector3.h"
#include <vector>
bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap);

float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z);

float GetYFromHeightMap(std::vector<unsigned char> &heightMap, Vector3& mapScale, float x, float z, float objectScaleY);

#endif