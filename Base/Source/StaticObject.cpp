#include "StaticObject.h"

StaticObject::StaticObject()
{
}

StaticObject::StaticObject(Mesh* mesh, Vector3 Pos, Vector3 scale, float Speed, float angle, bool canBePicked, GO_TYPE Type)
{
	/* Set object */
	Set("dfsdf", mesh, NULL, false, false);
	translateObject(Pos);	//set pos
	rotateObject(angle, 0, 0, 1);
	scaleObject(scale.x, scale.y, scale.z);	//set scale
	setActive(true); //set active

	/* set collide box */
	if(angle == 0)
	{
		collideBound.Set(Pos, scale, Collision::BOX);
	}
	else
		collideBound.Set(Pos, scale, Collision::SLANTED_BOX);

	/* set type */
	this->Type = Type;
}

StaticObject::~StaticObject()
{
}