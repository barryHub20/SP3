#ifndef CHARACTER_H
#define CHARACTER_H
#include "GameObject.h"

class Character : public GameObject
{
public:
	Character();
	Character(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active);
	~Character();
};

#endif