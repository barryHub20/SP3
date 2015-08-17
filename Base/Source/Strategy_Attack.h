#ifndef STRATEGY_ATTACK_H
#define STRATEGY_ATTACK_H
#include "Strategy.h"
/*****************************
For NPC use only.
if Strategy has a character pointer and character includes Strategy, circular dependency, how to resolve?
*****************************/

class Strategy_Attack : public Strategy
{
	static float patrol_dist;	//total length of patrol
	Vector2 middlePos;	//original pos
public:
	/* constructor/destructor */
	Strategy_Attack();
	virtual ~Strategy_Attack();

	/* Core */
	virtual void Init(const Vector2& targetPos, const Vector2& targetScale, Vector2& npc_pos, Vector2& npc_scale, Vector2& npc_velocity);
	virtual void Update(const Vector2& targetPos, const Vector2& targetScale, Vector2& npc_pos, Vector2& npc_scale, Vector2& npc_velocity, const Vector2& offset);	//update
};

#endif