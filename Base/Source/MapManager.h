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
	vector<Map*> MapList;
	Map *CurrentMap;
	int MapNo;
public:
	MapManager();
	~MapManager();

	void CreateMap(const int numOfTileWidth, const int numOfTileHeight, const int tileSize, const char* mapName);

	void SetMap(int Map);
	void SetMap(MAPS Map);
	Map* GetCurrentMap();
	void ChangeNextMap();
	void DeleteMap();
	void CleanUp();
};

#endif

