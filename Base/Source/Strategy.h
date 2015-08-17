#ifndef STRATEGY_H
#define STRATEGY_H
#include "Vector2.h"
#include <iostream>
using namespace std;
/*****************************
For NPC use only.
if Strategy has a character pointer and character includes Strategy, circular dependency, how to resolve?
*****************************/

class Strategy
{
public:
	Vector2 destination;	//pos to reach
	bool going_Left;
	bool going_Right;
	bool jumping_Up;
	bool jumping_Down;
	bool fireLeft;	//fire gun left
	bool fireRight;	//fire gun right

	enum STRATEGY_TYPE
	{
		PATROL,
		SHOOT,
		TOTAL_STRATEGY,
	};

	/* constructor/destructor */
	Strategy();
	virtual ~Strategy();

	/* Core */
	virtual void Init(const Vector2& targetPos, const Vector2& targetScale, Vector2& npc_pos, Vector2& npc_scale, Vector2& npc_velocity) = 0;
	virtual void Update(const Vector2& targetPos, const Vector2& targetScale, Vector2& npc_pos, Vector2& npc_scale, Vector2& npc_velocity, const Vector2& offset) = 0;	//update

	/* Getter/setter */
	Vector2 GetDestination();	//get pos of target
};

#endif