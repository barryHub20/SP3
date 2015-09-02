#pragma once
#include "GameObject.h"
#include "MapManager.h"
#include "PathFinding.h"

class Enemy : public GameObject
{
public:

	enum ENEMY_STATE
	{
		ES_START,
		ES_STOP,
		ES_WALK_LEFT,
		ES_WALK_RIGHT,
		ES_WALK_UP,
		ES_WALK_DOWN,
		ES_IDLE,
		ES_RETURN,
		ES_ALERT,
		ES_SCAN,
		ES_CHASE,
		ES_ATTACK,
		ES_ATTACK_UP,
		ES_ATTACK_DOWN,
		ES_ATTACK_LEFT,
		ES_ATTACK_RIGHT,
		ES_TOTAL
	};

	void setHealth(int Health);
	int getHealth();

	void setState(ENEMY_STATE State);
	int getState();

	void setName(string name);
	string getName();

	void setDestination(Vector2 Destination);
	Vector2 getDestination();

	void setDestinationReached(bool DestinationReached);
	bool getDestinationReached();

	virtual void Update(float dt,vector<Map*>* level_map,vector<GameObject*> goList);

	virtual void UpdateStateResponse(vector<Map*>* level_map, GameObject* Player) = 0;

	Enemy(void);
	~Enemy(void);

	ENEMY_STATE lastState;
public:
	PathFinding PathFinder;

private:

	Collision Detection;
	bool DestinationReached;
	Vector2 Destination;
	string Name;
	ENEMY_STATE State;
	int Health;
};

