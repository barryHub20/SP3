#include <iostream>
#include <fstream>
#include "LoadHmap.h"

bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if(!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}

    fileStream.seekg(0, std::ios::end);
    std::streampos fsize = (unsigned)fileStream.tellg();
	
	fileStream.seekg(0, std::ios::beg);
	heightMap.resize((unsigned)fsize);
	fileStream.read((char *)&heightMap[0], fsize);
	
	fileStream.close();
	return true;
}

//returns y value of particular region of terrain map (base on x and z, if out of range, return 0)
float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z)
{
	if(x < -0.5f || x > 0.5f || z < -0.5f || z > 0.5f)
	{
		return 0;
	}

	if(heightMap.size() == 0)
	{
		return 0;
	}

	//Teachers method code
	
	unsigned mapSize = static_cast<unsigned>(sqrt((double)heightMap.size()));
	unsigned xCoord = static_cast<unsigned>((x + 0.5f) * mapSize);
	unsigned zCoord = static_cast<unsigned>((z + 0.5f) * mapSize);
	
	return static_cast<float>(heightMap[zCoord * mapSize + xCoord] / 256.f);
	//Vincent's method code
	//float mapSize = sqrt((float)heightMap.size());

	//float _x = (x + 0.5f) * mapSize;
	//float _z = (z + 0.5f) * mapSize;

	//return heightMap[_x + _z * mapSize] / 256.f;
}

float GetYFromHeightMap(std::vector<unsigned char> &heightMap, Vector3& mapScale, float x, float z, float objectScaleY)
{
	return ReadHeightMap(heightMap, x / mapScale.x, z / mapScale.z) * mapScale.y + objectScaleY;
}