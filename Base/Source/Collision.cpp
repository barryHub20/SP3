#ifndef COLLISION_H
#define COLLISION_H
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



private:
	TYPE type;


};

#endif