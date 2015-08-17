#ifndef PLAYER_H
#define PLAYER_H
#include "Character.h"

class Player : public Character
{
	/* points */
	int score;
public:
	/* constructor / destrutor */
	Player();
	virtual ~Player();

	/* core */
	virtual void Set(Mesh* mesh, Vector3 scale, Vector3 position, float angle, Vector3 axis, float mass, Object* parent, bool light, int healthPoint, Gun::TYPE initType);
	void Update(bool keyPressedFire, bool keyPressedReload, bool* myKeys, const double dt);		//Controller::myKeys
	void changeWeapons(bool goDown, bool goUp);

	void increaseScore(int amt);
	int getScore();
};

#endif