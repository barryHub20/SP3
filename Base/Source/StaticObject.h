#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H
#include "GameObject.h"

//sofa, table etc
class StaticObject : public GameObject
{
	bool canBePicked;	//can this item be picked up>

public:
	StaticObject();
	StaticObject(Mesh* mesh, Vector3 Pos, Vector3 scale, float Speed, float angle, bool canBePicked, GO_TYPE Type);
	~StaticObject();
};

#endif