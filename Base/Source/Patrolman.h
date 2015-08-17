#ifndef PATROLMAN_H
#define PATROLMAN_H
#include "NPC.h"

class Patrolman : public NPC
{
public:
	const static int MAX_HEALTH = 5;
	int health;
	float patrolRange;

	/* constructor/destructor */
	Patrolman();
	~Patrolman();

	/* core */
	virtual void Init(SpriteAnimation* anim_mesh, float xPos, float yPos, float xScale, float yScale, Map* map, float patrolRange);
	void Update(double dt, Vector2& offset, Map* map, Character* enemy);

	/* movement */
	virtual void Jump(double dt);
	virtual void MoveLeftRight(bool mode, double dt);
};

#endif
