#include "NPC.h"

/* constructor/destructor */
NPC::NPC()
{
	offset.Set(0, 0);
	offsetPos = false;
	shotLeft = shotRight = false;
	fireLeft = fireRight = false;

	/* initialize strategy pack */
	for(int i = 0; i < Strategy::TOTAL_STRATEGY; ++i)
	{
		strategy_pack[i] = NULL;
	}
}

NPC::~NPC()
{
}

void NPC::Init(SpriteAnimation* anim_mesh, float xPos, float yPos, float xScale, float yScale, Map* map)
{
	/* parent init */
	Character::Init(anim_mesh, xPos, yPos, xScale, yScale, map);
	originalPos = pos;
}

void NPC::InitStrategy(Character* enemy)
{
	setStrategy(enemy);
}

void NPC::setStrategy(Character* enemy)
{
	/* create strategy */
	switch (currentStrategy)
	{
		case Strategy::PATROL:
			if(strategy_pack[Strategy::PATROL] == NULL)
			{
				Strategy_Patrol* strg_Patrol = new Strategy_Patrol;
				strategy_pack[Strategy::PATROL] = strg_Patrol;
				strategy_pack[Strategy::PATROL]->Init(enemy->pos, enemy->scale, pos, scale, velocity);
			}
			break;
		case Strategy::SHOOT:
			if(strategy_pack[Strategy::SHOOT] == NULL)
			{
				Strategy_Attack* strg_Shoot = new Strategy_Attack;
				strategy_pack[Strategy::SHOOT] = strg_Shoot;
				strategy_pack[Strategy::SHOOT]->Init(enemy->pos, enemy->scale, pos, scale, velocity);
			}
			break;
	}
}

void NPC::RunStrategy(double dt, Vector2& offset, Map* map, Character* enemy)
{
	if(strategy_pack[currentStrategy] != NULL)
		strategy_pack[currentStrategy]->Update(enemy->pos, enemy->scale, pos, scale, velocity, offset);
}

void NPC::Update(double dt, Vector2& offset, Map* map, Character* enemy)
{
	/* Update offset */
	Vector2 difference = this->offset - offset;

	if(!difference.isZero())	//if offset changes
	{
		offsetPos = true;
	}


	if(offsetPos)	//if offset changes, update pos
	{
		//pos = originalPos - offset;
		pos += difference;
		this->offset = offset;
		offsetPos = false;
	}

	if(pos.y + scale.y >= map->GetNumOfTiles_Height() * map->GetTileSize())
	{
		pos.y = map->GetNumOfTiles_Height() * map->GetTileSize() - scale.y;
		jumpSpeed = 0.f;
	}
}

/* movement */
void NPC::Jump(double dt)
{
}

void NPC::MoveLeftRight(bool mode, double dt)
{
}