/******************************************************************************/
/*!
\file	MapManager.h
\author Ong Rui Peng
\brief
Class to manage maps
*/
/******************************************************************************/
#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "Map.h"

class MapManager
{
public:
	enum MAPS
	{
		MAP1,
		MAP2,
		MAP3,
		MAX_MAP,
	};
private:
	vector<vector<Map*>> MapList; //Vector to store all maps
	vector<Map*> CurrentMap; //Pointer to current map
	int MapNo; //Track current map
public:
	MapManager();
	~MapManager();

	void Init();
	void CreateMap(MapManager::MAPS map, Map::TYPE mapType, int numOfTileWidth, const int numOfTileHeight, const int tileSize, const char * mapName, Mesh* tileSet, bool haveCollision = true); //Create map and add into maplist
	void CreateMapFloor(MapManager::MAPS map, Mesh* floorMesh);
	void AddRear(MapManager::MAPS map, Map::TYPE mapType, int numOfTileWidth, const int numOfTileHeight, const int tileSize, const char * mapName, Mesh* tileSet, bool haveCollision = true); //Add a rear map
	void SetMap(int Map); //Set map
	void SetMap(MAPS Map); //Set map
	vector<Map*>* GetCurrentMap(); //Get the current map
	void ChangeNextMap(); //Move to next map

	void SetMapNull(); //set current map to null
	void CleanUp(); //delete all maps
};

#endif

