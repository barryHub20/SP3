#include "Map.h"
#include <iostream>
using namespace std;

Map::Map(void)
: theScreen_Height(0)
, theScreen_Width(0)
, theNumOfTiles_Height(0)
, theNumOfTiles_Width(0)
, theTileSize(0)
{
	theScreenMap.clear();
	BoundBoxList.clear();
}

Map::~Map(void)
{
}

void Map::Init(const int num_of_tile_Width, const int num_of_tile_Height, const int tileSize)
{
	this->theTileSize			= tileSize;		//tile dimension
	this->theNumOfTiles_Height	= num_of_tile_Height; //how many tiles horizontal
	this->theNumOfTiles_Width	= num_of_tile_Width;	//how many tiles vertical


	this->theScreen_Height		= num_of_tile_Height * tileSize;		//screen height
	this->theScreen_Width		= num_of_tile_Width * tileSize;		//screen width

	theScreenMap.resize(theNumOfTiles_Height);	//set the number of rows

	for (int i = 0; i < theNumOfTiles_Height; ++i)
		theScreenMap[i].resize(theNumOfTiles_Width);	//set the number of column per row
}

bool Map::LoadMap(const string mapName)
{
	if (LoadFile(mapName) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

void Map::LoadBBox(void)
{
	int m = 0;
	BoundBox_3D box;
	//mapFineOffset_x = mapOffset_x % m_cMap->GetTileSize();
	for (int i = 0; i < GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < GetNumOfTiles_Width(); k++)
		{
			//m = tileOffset_x + k;
			Vector3 boxPos;
			Vector3 scale(theTileSize, theTileSize, theTileSize);
			if (theScreenMap[i][k] > 0)
			{
				boxPos.Set(k * theTileSize, i * theTileSize);	
				//cout << boxPos << endl;
				box.Set(boxPos, scale);
				BoundBoxList.push_back(box);
			}
		/*	else if (theScreenMap[i][k] == 5)
			{
				boxPos.Set(k * theTileSize, 575.f - i * theTileSize);
				box = new BoundingBox(boxPos, 25.f, 25.f, true);
				theOriginalBoxMap.push_back(box);
				theBoundingBoxMap.push_back(*box);
			}
			else if (theScreenMap[i][k] == 6)
			{
				boxPos.Set(k * theTileSize, 575.f - i * theTileSize);
				box = new BoundingBox(boxPos, 25.f, 25.f, true);
				secondBoundingBoxMap.push_back(*box);
				delete box;
			}*/
		}
	}
}

bool Map::LoadFile(const string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;

	ifstream file(mapName.c_str());
	if(file.is_open())
	{
		int i = 0, k = 0;
		while(file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			if  (theLineCounter>=theNumOfTiles_Height)
				break;

			// If this line is not a comment line, then process it
			if(!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				if (theLineCounter == 0)
				{
					// This is the first line of the map data file
					string token;
					istringstream iss(aLineOfText);
					int num = 0;
					while(getline(iss, token, ',') && num < theNumOfTiles_Width)
					{
						// Count the number of columns
						++num;
					}
					theMaxNumOfColumns = theNumOfTiles_Width;
				}
				else
				{
					int theColumnCounter = 0;

					string token;
					istringstream iss(aLineOfText);
					while(getline(iss, token, ',') && (theColumnCounter<theNumOfTiles_Width))
					{
						theScreenMap[(theNumOfTiles_Height - 1) - theLineCounter][theColumnCounter++] = atoi(token.c_str());
					}
				}
			}

			theLineCounter++;
		}
	}
	return true;
}

int Map::GetNumOfTiles_Height(void)
{
	return theNumOfTiles_Height;
}

int Map::GetNumOfTiles_Width(void)
{
	return theNumOfTiles_Width;
}

int Map::GetTileSize(void)
{
	return theTileSize;
}

vector<BoundBox_3D> Map::GetBBList(void)
{
	return BoundBoxList;
}

void Map::CleanUp(void)
{
	BoundBoxList.clear();
	theScreenMap.clear();
}
