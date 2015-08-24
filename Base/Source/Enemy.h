#pragma once
#include "GameObject.h"
#include "MapManager.h"

class Enemy : public GameObject
{
public:

	enum ENEMY_STATE
	{
		ES_START,
		ES_ESCAPE,
		ES_WALK_LEFT,
		ES_WALK_RIGHT,
		ES_WALK_UP,
		ES_WALK_DOWN,
		ES_IDLE,
		ES_RETURN,
		ES_ALERT,
		ES_SCAN,
		ES_KNIFESTAB,
		ES_GUNSHOT,
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

	virtual void Update(float dt,MapManager *mapManager,vector<GameObject*> goList);

	virtual void UpdateStateResponse(MapManager *mapManager, GameObject* Player) = 0;

	Enemy(void);
	~Enemy(void);

private:
	Collision Detection;
	bool DestinationReached;
	Vector2 Destination;
	string Name;
	ENEMY_STATE State;
	int Health;
};

