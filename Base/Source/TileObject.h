#ifndef TILE_OBJECT_H
#define TILE_OBJECT_H
#include "GameObject.h"

class TileObject : public GameObject
{
public:
	enum TILE_TYPE
	{
		FLOOR,	//is floor
		WALL,	//is wall
	};

	/* constructor/destructor */
	TileObject();
	~TileObject();

	/* Core */
	void Set(Mesh* mesh, Vector3 Pos, float tileScale);

	/* getter/setter */
	int getTileNum();
	void setTileNum(int t);
	
	/* collision check */
	virtual bool CollisionCheck(GameObject* checkWithMe);	//collision check

private:
	TILE_TYPE tile_type;
	int tileNum;	//tile number
};

#endif