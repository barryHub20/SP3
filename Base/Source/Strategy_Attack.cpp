#include "Strategy_Attack.h"

float Strategy_Attack::patrol_dist = 200.f;

/* constructor/destructor */
Strategy_Attack::Strategy_Attack()
{
}

Strategy_Attack::~Strategy_Attack()
{
}

/* Core */
void Strategy_Attack::Init(const Vector2& targetPos, const Vector2& targetScale, Vector2& npc_pos, Vector2& npc_scale, Vector2& npc_velocity)
{
	going_Left = true;
	going_Right = false;
}

void Strategy_Attack::Update(const Vector2& targetPos, const Vector2& targetScale, Vector2& npc_pos, Vector2& npc_scale, Vector2& npc_velocity, const Vector2& offset)
{
	fireLeft = false;
	fireRight = false;
	
	/* fire on player */
	if(targetPos.x < npc_pos.x)	//left
		fireLeft = true;
	else	//right
		fireRight = true;

	/* y pos oso update */
	npc_pos.y += npc_velocity.y;
}