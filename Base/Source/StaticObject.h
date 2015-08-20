#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H
#include "GameObject.h"

//sofa, table, wall etc
class StaticObject : public GameObject
{
public:
	StaticObject();
	StaticObject(Mesh* mesh, Vector3 Pos, Vector3 scale, float Speed, float angle, bool canBePicked, GO_TYPE Type);
	~StaticObject();
};

#endif