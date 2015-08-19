#include "Player.h"

Player::Player()
{
}

Player::Player(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active) : Character(mesh, Pos, scale, angle, Speed, active)
{
	score = 0;
	Type = GO_PLAYER;
	jumpSpeed = 0;
	PlayerOnGround = true;
	PlayerInAir = false;
}

Player::~Player()
{
}

void Player::Update(double dt, bool* myKey, float x_limit, float y_limit)
{
	// Movement
	if(myKey[KEY_W])
	{
		if (position.y < y_limit)
		translateObject(0, 10, 0);
	}

	if(myKey[KEY_S])
	{
		if (position.y > scale.x*0.25f)
		translateObject(0, -10, 0);
	}	

	if(myKey[KEY_A])
	{
		if (position.x > scale.x*0.25f)
		translateObject(-10, 0, 0);
	}

	if(myKey[KEY_D])
	{
		if (position.x < x_limit)
		translateObject(10, 0, 0);
	}

	if(myKey[KEY_SPACE])
	{
		PlayerInAir = true;
		PlayerOnGround = false;
	}

	if(PlayerInAir == true && PlayerOnGround == false)
	{
		jumpSpeed += dt;
		if(jumpSpeed >= 10)
		{
			jumpSpeed = 10;
		}
	}
}

void setScore(int amtScore)
{
	
}

void Player::CollisionResponse()
{
	if(collided)
	{
		translate(collideBound.position);
	}
}

int Player::getScore()
{
	return score;
}