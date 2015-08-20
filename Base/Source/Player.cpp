#include "Player.h"

Player::Player()
{
}

Player::Player(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active)
{
	score = 0;
	
	/* set object */
	Set("sdfdf", mesh, NULL, false, false);
	translateObject(Pos.x, Pos.y, Pos.z);
	scaleObject(scale.x, scale.y, scale.z);

	/* set angle */
	angleZ = angle;

	/* set physics */
	info.setSpeed(Speed);
	info.setDir(Vector2(1, 0));	//should be based on angle

	/* set boundbox */
	collideBound.Set(Pos, scale, Collision::BOX);

	jumpSpeed = 0;
	PlayerOnGround = true;
	PlayerInAir = false;
}

Player::~Player()
{
}

void Player::Update(double dt, bool* myKey)
{
	if(myKey[KEY_W])
	{
		translateObject(0, 4, 0);
	}

	if(myKey[KEY_S])
	{
		translateObject(0, -4, 0);
	}	

	if(myKey[KEY_A])
	{
		translateObject(-4, 0, 0);
	}

	if(myKey[KEY_D])
	{
		translateObject(4, 0, 0);
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