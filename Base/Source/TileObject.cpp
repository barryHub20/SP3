#include "TileObject.h"

/* constructor/destructor */
TileObject::TileObject()
{
}

TileObject::~TileObject()
{
}

/* Core */
void TileObject::Set(Mesh* mesh, Vector3 Pos, float tileScale)
{
	/* Set object */
	Object::Set("dfsdf", mesh, NULL, false, false);
	translateObject(Pos);	//set pos
	scaleObject(tileScale, tileScale, 1);	//set scale
	setActive(true); //set active

	/* set collide box */
	collideBound.Set(Pos, scale, Collision::BOX);
}

/* getter/setter */
int TileObject::getTileNum()
{
	return tileNum;
}

void TileObject::setTileNum(int t)
{
	tileNum = t;
}

/* collision check */
bool TileObject::CollisionCheck(GameObject* checkWithMe)
{
	if(this->tile_type == FLOOR)	//floor does not have collision
		return false;

	return GameObject::CollisionCheck(checkWithMe);
}
