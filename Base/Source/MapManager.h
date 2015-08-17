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
	enum MAPS
	{
		MAP1,
		MAP2,
		MAP3,
		MAX_MAP,
	};
private:
	vector<Map*> MapList; //Vector to store all maps
	Map *CurrentMap; //Pointer to current map
	int MapNo; //Track current map
public:
	MapManager();
	~MapManager();

	void CreateMap(const int numOfTileWidth, const int numOfTileHeight, const int tileSize, const char* mapName); //Create map and add into maplist
	void UpdateBBoxList(); //Update bounding box list
	void SetMap(int Map); //Set map
	void SetMap(MAPS Map); //Set map
	Map* GetCurrentMap(); //Get the current map
	void ChangeNextMap(); //Move to next map

	void ResetBBoxList(); //Clear bbox list
	void SetMapNull(); //set current map to null
	void CleanUp(); //delete all maps

	vector<BoundBox_3D> CurrentBBox; //Update the bounding box list to current map
};

#endif

