#include "Player.h"

Player::Player()
{
}

Player::Player(Vector3 Pos, Vector3 scale, Vector3 Dir, float Speed, bool active) : GameObject(Pos, scale, Dir, Speed, active)
{
	score = 0;
}

Player::~Player()
{
}

void Player::Update(double dt, bool* myKey)
{
	if(myKey[KEY_W])
	{
		object.translateObject(Vector3(0, 10, 0));
	}

	if(myKey[KEY_S])
	{
		object.translateObject(Vector3(0, -1, 0));
	}	

	if(myKey[KEY_A])
	{
		object.translateObject(Vector3(1, 0, 0));
	}

	if(myKey[KEY_A])
	{
		object.translateObject(Vector3(0, 0, 1));
	}
}

void setScore(int amtScore)
{
	
}

int Player::getScore()
{
	return score;
}