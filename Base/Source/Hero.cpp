#include "Hero.h"
const float JUMP_SPEED = 7.f;
/* constructor/destructor */

Hero::Hero()
{
}

Hero::~Hero()
{
}

/* core */
void Hero::Init(SpriteAnimation* anim_mesh, float xPos, float yPos, float xScale, float yScale, Map* map)
{
	/* parent init */
	Character::Init(anim_mesh, xPos, yPos, xScale, yScale, map);

	moveSpeed = 90.f;
	health = MAX_HEALTH;
	fire = false;
	shotLeft = shotRight = false;
	FireRate = 0.2f;
	FireTimer = FireRate;
}

void Hero::Constrain(float m_window_width, float m_window_height, Vector2& middlePos, const float constrain_DistX, Vector2& offset, Map* map)
{
	float dist_offset = 0.1f;	//for max dist. before new tile (new tile origin - offset)

	if(pos.x <= middlePos.x - constrain_DistX)	//left
	{
		offset.x -= (middlePos.x - constrain_DistX) - pos.x;

		/* not reach edge */
		if(offset.x >= 0.f)
			pos.x = middlePos.x - constrain_DistX;

		/* reach edge */
		else
		{
			if(pos.x < 0.f)
				pos.x = 0.f;
			offset.x = 0.f;
		}
	}

	else if(pos.x + scale.x >= middlePos.x + constrain_DistX)	//right
	{
		offset.x += (pos.x + scale.x) - (middlePos.x + constrain_DistX);

		/* if not reach edge, max offset when collide right is (map_width - window_width) */
		if(offset.x <= (map->GetNumOfTiles_Width() * map->GetTileSize()) - m_window_width)
			pos.x = (middlePos.x + constrain_DistX) - scale.x;
		
		/* reach edge */
		else
		{
			if(pos.x + scale.x >= m_window_width)	//reach edge of map?
				pos.x = m_window_width - scale.x - dist_offset;	//inc. - offset since

			offset.x = (map->GetNumOfTiles_Width() * map->GetTileSize()) - m_window_width;
		}
	}

	/** Y constrain **/
	if(pos.y + scale.y >= m_window_height)
	{
		pos.y = m_window_height - scale.y - dist_offset;
		jumpSpeed = 0.f;
		velocity.y = 0.f;
	}
	else if(pos.y <= 0.f)
	{
       pos.y = 0.1f;	//floating offset               
	}
}

//for detecting if pressed once already or not
bool pressed_Left = false;
bool pressed_Right = false;
bool no_pressed = false;	//if never press either
void Hero::Update(double dt, bool* myKeys, float m_window_width, float m_window_height, Vector2& middlePos, const float constrain_DistX, Vector2& offset, Map* map)
{
	velocity.x = 0;
	fire = false;

	if(FireTimer < FireRate)
		FireTimer += dt;
	else
	{
		if(myKeys[KEY_O])
		{
			FireTimer = 0.f;
			fire = true;
		}	
	}

	if(shotLeft || shotRight)
	{
		health--;
		shotLeft = shotRight = false;
	}

	/* control hero and change animation each time we change stance */
	if(myKeys[KEY_A] && !left_Collide_Wall)
	{
		velocity.x = -dt * 180.5f;
		c_movingRight = false;
		c_movingLeft = true;

		if(!pressed_Left)	//pressed
		{
			pressed_Left = true;
			anim_mesh->init(1.5f, 0, 1, 9, 1, false);
			invertAnim = true;
		}
	}
	else
		pressed_Left = false;


	if(myKeys[KEY_D] && !right_Collide_Wall)
	{
		velocity.x = dt * 180.5f;
		c_movingRight = true;
		c_movingLeft = false;

		if(!pressed_Right)
		{
			pressed_Right = true;
			anim_mesh->init(1.5f, 0, 0, 9, 0, false);
			invertAnim = false;
		}
	}
	else
		pressed_Right = false;


	//not moving
	if(!myKeys[KEY_A] && !myKeys[KEY_D])
	{
		if(!no_pressed)
		{
			anim_mesh->init(52.5f, 0, 0, 0, 0, false);
		}
	}
	else
		no_pressed = false;

	
	if(myKeys[KEY_SPACE])
	{
		if (!c_jumpingUp && !c_jumpingDown)
		{
			c_jumpingUp = true;
			jumpSpeed = JUMP_SPEED;
		}
	}

	/* if flags still true, do action */
	/*** While hero is jumping up/down, update its Pos and speed ***/
	if(c_jumpingUp && jumpSpeed <= Math::EPSILON && -jumpSpeed >= Math::EPSILON)
	{
		jumpSpeed = 0.f;
		velocity.y = 0.f;
		c_jumpingDown = true;
		c_jumpingUp = false;
	}

	if(c_jumpingUp || c_jumpingDown)
	{
		jumpSpeed -= 0.7;
		velocity.y += jumpSpeed;
	}

	/* Update pos */
	pos += velocity;

	/* check collision: if collide, set respective flag to false */
	// constrain 
	Constrain(1024, 800, middlePos, constrain_DistX, offset, map);

	// update collision 
	Collide(offset, map);
}

/* movement */
void Hero::Jump(double dt)
{
}

void Hero::MoveLeftRight(bool mode, double dt)
{
}