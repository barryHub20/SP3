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
		DIRT = 1,
		GOLD = 2,
		IRON_ORE = 3,
		REDSTONE_ORE = 4,
		BRICK = 5,
		DIAMOND = 6,
		EMRRALD_ORE = 7,
		GOLD_ORE = 8,
		OAK_PLANK = 9,
		OAK_WOOD = 10,
		OAK_LEAF = 11,
		TOTAL_TYPE
	};

	Map(void);
	~Map(void);

	void Init(const int num_of_tile_Width, const int num_of_tile_Height, const int tileSize, Mesh* tileSet);

	int GetNumOfTiles_Height(void);
	int GetNumOfTiles_Width(void);
	int GetTileSize(void);

	bool LoadMap(const string mapName);	//load from csv to double vector
	bool SetUp();	//setup each tile

	//RP
	void CleanUp(void); //reset tile object list list
	TileObject* getTileObject(int x, int y);	//get tile based on x and y
	void CheckCollisionWith(GameObject* checkWithMe);	//Utility function: pass in to check collision with this map
	bool getWalkable(int x, int y);
private:
	int theScreen_Height;
	int theScreen_Width;
	int theNumOfTiles_Height;
	int theNumOfTiles_Width;
	int theTileSize;

	//RP
	Mesh* tileSet;
	vector<vector<TileObject> > theScreenMap;
	
	bool LoadFile(const string mapName);
};
