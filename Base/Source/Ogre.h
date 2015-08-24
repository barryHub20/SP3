#pragma once
#include "Player.h"
#include "Enemy.h"

class Ogre : public Enemy
{
private:
	Collision DetectionBound;

public:

	void setDetecionBound();
	Collision getDetecionBound();

	void Update(float dt,MapManager *mapManager,vector<GameObject*> goList);
	void UpdateStateResponse(MapManager *mapManager,GameObject* Player);

	Ogre(void);
	Ogre(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active);
	~Ogre(void);
};

