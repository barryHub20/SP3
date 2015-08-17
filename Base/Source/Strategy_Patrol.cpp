#include "Strategy_Patrol.h"
float Strategy_Patrol::patrol_dist = 200.f;

/* constructor/destructor */
Strategy_Patrol::Strategy_Patrol()
{
}

Strategy_Patrol::~Strategy_Patrol()
{
}

/* Core */
void Strategy_Patrol::Init(const Vector2& targetPos, const Vector2& targetScale, Vector2& npc_pos, Vector2& npc_scale, Vector2& npc_velocity)
{
	/** See if need go left or right **/
	going_Right = false;
	going_Left = true;
	middlePos.Set(0, 0);
	middlePos.x = npc_pos.x;

	/* Set destination */
	if(going_Left)
		destination.x = npc_pos.x - patrol_dist * 0.5f;
	else
		destination.x = npc_pos.x + patrol_dist * 0.5f;
}

void Strategy_Patrol::Update(const Vector2& targetPos, const Vector2& targetScale, Vector2& npc_pos, Vector2& npc_scale, Vector2& npc_velocity, const Vector2& offset)
{
	/* Go to the left */
	if(going_Left)
	{
		//keep updating destination with offset
		destination.x = (middlePos.x - offset.x) - patrol_dist * 0.5f;	
		npc_pos.x -= npc_velocity.x;

		/* if reach */
		if(npc_pos.x - npc_velocity.x <= destination.x)
		{
			//npc_pos.x = destination.x;
			destination.x += patrol_dist;
			going_Left = false;
			going_Right = true;
		}
	}

	/* Go to the right */
	else if(going_Right)
	{
		destination.x = (middlePos.x - offset.x) + patrol_dist * 0.5f;
		npc_pos.x += npc_velocity.x;

		/* if reach */
		if(npc_pos.x + npc_velocity.x >= destination.x)
		{
			//npc_pos.x = destination.x;
			destination.x -= patrol_dist;
			going_Left = true;
			going_Right = false;
		}
	}

	/* y pos oso update */
	npc_pos.y += npc_velocity.y;
}