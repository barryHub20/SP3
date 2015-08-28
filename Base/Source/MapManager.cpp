#include "MapManager.h"



MapManager::MapManager()
{
	CurrentMap.clear();
	MapList.clear();
	MapNo = NULL;
}

MapManager::~MapManager()
{
}

void MapManager::Init()
{
	MapList.resize(MAX_MAP);
}

void MapManager::CreateMap(MapManager::MAPS map, Map::TYPE mapType,const int numOfTileWidth, const int numOfTileHeight, const int tileSize, const char * mapName, Mesh* tileSet, bool haveCollision)
{
	Map *tempMap;
	tempMap = new Map();
	tempMap->Init(numOfTileWidth, numOfTileHeight, tileSize, tileSet); //Init new map
	tempMap->LoadMap(mapName); //Load it
	tempMap->SetUp(haveCollision); //set up
	tempMap->setMapType(mapType);
	MapList[map].push_back(tempMap);
	//MapList.push_back(tempMap); //Push it back into map list
}	

void MapManager::CreateMapFloor(MapManager::MAPS map, int numOfTileWidth, const int numOfTileHeight, const int tileSize, Mesh* floorMesh)
{
	Map *tempMap;
	tempMap = new Map();
	tempMap->Init(numOfTileWidth, numOfTileHeight, tileSize, NULL);
	tempMap->setMapType(Map::FLOORMAP);
	tempMap->createFloor(floorMesh);
	MapList[map].push_back(tempMap);
}

void MapManager::AddRear(MapManager::MAPS map, Map::TYPE mapType, int numOfTileWidth, const int numOfTileHeight, const int tileSize, const char * mapName, Mesh* tileSet, bool haveCollision)
{
	if (MapList[map][0] == NULL)
	{
		cout << "No main map" << endl;
	}
	else
	{
		Map *tempMap;
		tempMap = new Map();
		tempMap->Init(numOfTileWidth, numOfTileHeight, tileSize, tileSet); //Init new map
		tempMap->LoadMap(mapName); //Load it
		tempMap->SetUp(haveCollision); //set up
		tempMap->setMapType(mapType);
		MapList[map].push_back(tempMap);
	}
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

vector<Map*>* MapManager::GetCurrentMap()
{
	return &CurrentMap;
}

vector<Map*>* MapManager::GetMap(int index)
{
	if(index < MapList.size())
	{
		return &MapList[index];
	}
	else
	{
		return NULL;
	}
}

void MapManager::ChangeNextMap()
{
	++MapNo;
	if (MapNo >= MAX_MAP)
	{
		MapNo = MAP1;
	}

	CurrentMap = MapList[MapNo]; //Move to next map
	cout << "Changing map to " << MapNo << endl;
}

void MapManager::SetMapNull()
{
	for (int i = 0; i < CurrentMap.size(); i++)
	{
		CurrentMap[i] = NULL;
	}	
}

void MapManager::CleanUp()
{
	for (int i = 0; i < MapList.size(); i++)
	{
		for (int j = 0; j < MapList[i].size(); j++)
		{
			MapList[i][j]->CleanUp();
			delete MapList[i][j];
		}
	}
	SetMapNull();
}

