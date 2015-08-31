#pragma once
#include "Player.h"
#include "Enemy.h"

class Guard : public Enemy
{
private:
	Collision DetectionBound;
	bool detectedPlayer;
	vector<Vector3*> RouteList;
public:
	Collision getDetecionBound();
	
	virtual void StartCollisionCheck();
	
	void setDetecionBound();
	void CollisionResponse();
	void Update(float dt,vector<Map*>* level_map,vector<GameObject*>& goList);
	void UpdateStateResponse(vector<Map*>* level_map,GameObject* Player);

	Guard(void);
	Guard(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active);
	~Guard(void);
};

