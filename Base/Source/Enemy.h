#pragma once
#include "GameObject.h"
#include <string>

using std::string;

class Enemy : public GameObject
{
public:

	enum ENEMY_STATE
	{
		ES_IDLE,
		ES_RETURN,
		ES_SCAN,
		ES_ALERT,
		ES_ESCAPE,
		ES_GUNSHOT,
		ES_KNIFESTAB,
		ES_HURT,
	};

	void setName(string Name);
	void setState(ENEMY_STATE State);
	
	string getName(void);
	int getState(void);

	Enemy(void);
	~Enemy(void);

private:
	string Name;
	ENEMY_STATE State;

};

