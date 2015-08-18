#include "MapManager.h"



MapManager::MapManager()
{
	CurrentMap = NULL;
	MapNo = NULL;
}

MapManager::~MapManager()
{
}

void MapManager::CreateMap(const int numOfTileWidth, const int numOfTileHeight, const int tileSize, const char* mapName)
{
	Map *tempMap;
	tempMap = new Map();
	tempMap->Init(numOfTileWidth, numOfTileHeight, tileSize); //Init new map
	tempMap->LoadMap(mapName); //Load it
	tempMap->LoadBBox(); //Create bounding boxes for it
	MapList.push_back(tempMap); //Push it back into map list
}

void MapManager::UpdateBBoxList()
{
	CurrentBBox = CurrentMap->GetBBList(); //Get the bbox list from map and store
}

void MapManager::SetMap(int Map)
{
	CurrentMap = MapList[Map];
	ResetBBoxList(); //Reset the bbox list
	UpdateBBoxList(); //Set the bbox list to the setted map
	MapNo = Map;
}

void MapManager::SetMap(MAPS Map)
{
	CurrentMap = MapList[Map];
	ResetBBoxList(); //Reset the bbox list
	UpdateBBoxList(); //Set the bbox list to the setted map
	MapNo = Map;
}

Map * MapManager::GetCurrentMap()
{
	return CurrentMap;
}

void MapManager::ChangeNextMap()
{
	++MapNo;
	if (MapNo >= MAX_MAP)
	{
		MapNo = MAP1;
	}
	CurrentMap = MapList[MapNo]; //Move to next map
	ResetBBoxList(); //Reset the bbox lis
	UpdateBBoxList(); //Update bbox list according to new map
}

void MapManager::ResetBBoxList()
{
	CurrentBBox.clear();
}

void MapManager::SetMapNull()
{
	CurrentMap = NULL;
}

void MapManager::CleanUp()
{
	for (int i = 0; i < MapList.size(); i++)
	{
		MapList[i]->CleanUp();
		delete MapList[i];
	}
	CurrentMap = NULL;
}
