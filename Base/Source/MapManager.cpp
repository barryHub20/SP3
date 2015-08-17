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
	tempMap->Init(numOfTileWidth, numOfTileHeight, tileSize);
	tempMap->LoadMap(mapName);
	MapList.push_back(tempMap);
}

void MapManager::SetMap(int Map)
{
	CurrentMap = MapList[Map];
	MapNo = Map;
}

void MapManager::SetMap(MAPS Map)
{
	CurrentMap = MapList[Map];
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
	CurrentMap = MapList[MapNo];
}

void MapManager::DeleteMap()
{
	CurrentMap = NULL;
}

void MapManager::CleanUp()
{
	for (int i = 0; i < MapList.size(); i++)
	{
		delete MapList[i];
	}
	CurrentMap = NULL;
}
