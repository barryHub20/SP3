#pragma once

#include <ostream>
#include "Vector3.h"
#include <vector>

using std::vector;


class Collision
{
public:
	enum TYPE
	{
		BOX,
		SPHERE,
		SLANTED_BOX,
	};
	
	/* Check if collide */
	bool CheckCollision(Collision& check);
	bool CheckSphereDetection(Collision& check);
	bool CheckBoxDetection(Collision& check);
	

	Collision();
	~Collision();
private:
	TYPE type;

	/* flag variables */
	Vector3 scale;	//if 2D, z == 0
	Vector3 position;

	//aabb
	Vector3 start;
	Vector3 end;// PUSH TEST IF GT PUSH FUCK GIT

	/* internal variables */
	Vector3 previousPos;
	Vector3 previousStart;
	Vector3 previousEnd;
};
