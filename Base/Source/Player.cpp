#include "Player.h"

Player::Player()
{
}

Player::Player(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active) : Character(mesh, Pos, scale, angle, Speed, active)
{
	score = 0;
	Type = GO_PLAYER;
}

Player::~Player()
{
}

void Player::Update(double dt, bool* myKey)
{
	if(myKey[KEY_W])
	{
		translateObject(0, 10, 0);
	}

	if(myKey[KEY_S])
	{
		translateObject(0, -10, 0);
	}	

	if(myKey[KEY_A])
	{
		translateObject(-10, 0, 0);
	}

	if(myKey[KEY_D])
	{
		translateObject(10, 0, 0);
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