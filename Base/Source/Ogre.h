#pragma once
#include "Player.h"
#include "Enemy.h"

class Ogre : public Enemy
{
private:
	Collision DetectionBound;
	bool detectedPlayer;

	//PathFinding Variables *Do Not Edit*
	vector<Cell *> RouteList; // need
	int RouteToTake; // need
	Vector3 Route; // need

	bool isDestReached; // up to u
	bool isChasing; // up to u
public:

	Collision getDetecionBound();
	
	virtual void StartCollisionCheck();
	
	void setDetecionBound();
	void CollisionResponse();
	void Update(float dt,vector<Map*>* level_map,vector<GameObject*>& goList);
	void UpdateStateResponse(vector<Map*>* level_map,GameObject* player);

	Ogre(void);
	Ogre(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active);
	~Ogre(void);
};

