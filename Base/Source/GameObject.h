#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Physics.h"


class GameObject
{
	public:

	enum GO_TYPE
	{
		GO_START,
		GO_FURNITURE,
		GO_ITEM,
		GO_TILE,
		GO_PLAYER,
		GO_ENEMY,
		GO_CAMERA,
		GO_LASER,
		GO_TOTAL,
	};

	void setInfo(Vector3 Pos, Vector3 Dir, int Speed, int Force);
	void setIsActive(bool isActive);
	void setType(GO_TYPE Type);

	Physics getInfo();
	int getObjCount();
	bool getisActive();
	int getType();
	
	GameObject(void);
	~GameObject(void);

private:

	static int objCount;

	Physics Info;
	bool isActive;

	GO_TYPE Type;
};

#endif 

