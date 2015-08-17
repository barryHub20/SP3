#include "Patrolman.h"
const int JUMP_SPEED = 20.f;
float dist_from_enemyX = 250.f;
float dist_from_enemyY = 100.f;

/* constructor/destructor */
Patrolman::Patrolman()
{
}

Patrolman::~Patrolman()
{
}

void Patrolman::Init(SpriteAnimation* anim_mesh, float xPos, float yPos, float xScale, float yScale, Map* map, float patrolRange)
{
	/* parent init */
	NPC::currentStrategy = Strategy::PATROL;	//set currentStrategy before init NPC,

	NPC::Init(anim_mesh, xPos, yPos, xScale, yScale, map);

	this->patrolRange = patrolRange;
	velocity.Set(1, 0);
	dist_From_Enemy = 200.f;
	moveSpeed = 2.f;

	fireRate = 1.0;
	fireTimer = fireRate;

	health = MAX_HEALTH;
}

void Patrolman::Update(double dt, Vector2& offset, Map* map, Character* enemy)
{
	if(health <= 0)
	{
		active = false;
		return;
	}

	/* Set velocity with dt */
	velocity.Set(30 * dt, 0);

	/* offset update */
	NPC::Update(dt, offset, map, enemy);

	/* Movement */
	if(strategy_pack[currentStrategy]->going_Left)
	{
		c_movingRight = false;
		c_movingLeft = true;
	}
	else if(strategy_pack[currentStrategy]->going_Right)
	{
		c_movingLeft = false;
		c_movingRight = true;
	}

	/* if shot */
	if(shotLeft)	//knockback
	{
		velocity.x *= 5.f;
		c_jumpingUp = true;
		jumpSpeed = JUMP_SPEED;

		--health;
		shotLeft = false;
	}
	else if(shotRight)	//knockback
	{
		velocity.x *= -5.f;
		c_jumpingUp = true;
		jumpSpeed = JUMP_SPEED;

		--health;
		shotRight = false;
	}

	//collide with a block/wall, jump to move on
	if(  (xCollide_origin > -1 || xCollide_end > -1) && (yCollide_origin <= -1 && yCollide_end <= -1) )
	{
		if (!c_jumpingUp && !c_jumpingDown)
		{
			c_jumpingUp = true;
			jumpSpeed = JUMP_SPEED;
		}
	}

	if(c_jumpingUp && jumpSpeed <= Math::EPSILON && -jumpSpeed >= Math::EPSILON)
	{
		jumpSpeed = 0.f;
		velocity.y = 0.f;
		c_jumpingDown = true;
		c_jumpingUp = false;
	}

	if(c_jumpingUp || c_jumpingDown)
	{
		jumpSpeed -= 2.f;
		velocity.y += jumpSpeed;
	}

	/* strategy update */
	NPC::RunStrategy(dt, offset, map, enemy);

	/* change strategy */
	//in range: attack
	if( (pos.x - enemy->pos.x) >= -dist_from_enemyX &&
		(pos.x - enemy->pos.x) <= dist_from_enemyX &&
		(pos.y - enemy->pos.y) >= -dist_from_enemyY &&
		(pos.y - enemy->pos.y) <= dist_from_enemyY )
	{
		NPC::currentStrategy = Strategy::SHOOT;
		setStrategy(enemy);
	}

	//Not in range: do not attack
	else
	{
		NPC::currentStrategy = Strategy::PATROL;
		setStrategy(enemy);
	}

	/* Do strategy specific task */
	if(currentStrategy == Strategy::SHOOT)
	{
		fireLeft = fireRight = false;

		if(fireTimer < fireRate)
			fireTimer += dt;
		else
		{
			fireTimer = 0.0;

			if(strategy_pack[currentStrategy]->fireLeft)
				fireLeft = true;
			else if(strategy_pack[currentStrategy]->fireRight)
				fireRight = true;
		}
	}
	else if(currentStrategy == Strategy::PATROL)
	{
		//if collide a wall, jump
		fireTimer = 0.0;
	}

	//update collision 
	Collide(offset, map);
}

/* movement */
void Patrolman::Jump(double dt)
{
}

void Patrolman::MoveLeftRight(bool mode, double dt)
{
}