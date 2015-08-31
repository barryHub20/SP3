#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "TileObject.h"

using namespace std;

class Map
{
public:
	//type
	enum TYPE
	{
		NOTHING = 0,
		FLOORMAP,
		COLLISIONMAP,
		NOCOLLISIONMAP,
		TOTAL_TYPE
	};

	Map(void);
	~Map(void);

	void Init(const int num_of_tile_Width, const int num_of_tile_Height, const int tileSize, Mesh* tileSet);

	int GetNumOfTiles_Height(void);
	int GetNumOfTiles_Width(void);
	int GetTileSize(void);

	bool LoadMap(const string mapName);	//load from csv to double vector
	bool SetUp(bool haveCollision = true);	//setup each tile ( bounding box )

	//Ray
	vector<vector<TileObject>>* getScreenMap();
	TileObject* getTile(int y, int x);	//put y first

	//RP
	void CleanUp(void); //reset tile object list list
	void createFloor(Mesh* floorMesh);
	Mesh* getFloorMesh();
	void setMapType(Map::TYPE mapType);
	Map::TYPE getMapType();

	//Yie cher
	TileObject* getTileObject(int x, int y);	//get tile based on x and y
	bool CheckCollisionWith(GameObject* checkWithMe);	//Utility function: pass in to check collision with this map RETURN WRONG
	bool QuickAABBCheckMap(GameObject* checkWithMe);
	bool getWalkable(int x, int y);
private:
	int theScreen_Height;
	int theScreen_Width;
	int theNumOfTiles_Height;
	int theNumOfTiles_Width;
	int theTileSize;

	//RP
	Mesh* tileSet;
	Mesh* floorMesh;
	TYPE mapType;

	//Cher
	vector<vector<TileObject> > theScreenMap;
	
	bool LoadFile(const string mapName);
};
