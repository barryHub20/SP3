#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "SpriteAnimation.h"
#include <vector>

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float length = 1.f, float texCoord = 1.f);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateDebugCube(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR = 1.f, float innerR = 0.f);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* GenerateText(const std::string &meshName, unsigned row, unsigned col);
	static Mesh* GenerateCrossHair(const std::string &meshName, float colour_r=1.0f, float colour_g=1.0f, float colour_b=0.0f, float length=1.0f);
	//hTile: horizontal texcoord tile, vTile: vertical texcoord tile
	static Mesh* GenerateSkyPlane(const std::string &meshName, Color color, int slices,float PlanetRadius, float AtmosphereRadius, float hTile, float vTile);
	static Mesh* GenerateTerrain(const std::string &meshName, const std::string &file_path, std::vector<unsigned char> &heightMap);
	static SpriteAnimation* MeshBuilder::GenerateSpriteAnimation(const std::string& meshName, unsigned numRow, unsigned numCol, float length);

	static Mesh* GenerateMinimap(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateMinimapBorder(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateMinimapAvatar(const std::string &meshName, Color color, float length = 1.f);

	/* 2D */
	static Mesh* Generate2DMesh(const std::string &meshName, Color color, int width, int height);
	static Mesh* GenerateTileMap(const std::string &meshName, Color color, int width, int height, unsigned numRow, unsigned numCol);
};

#endif