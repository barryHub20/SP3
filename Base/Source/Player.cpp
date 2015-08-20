#include "Player.h"

#include "irrKlang.h"
using namespace irrklang;

extern ISoundEngine* sfxengine;		//
extern ISound *sfx_plyr_step;

Player::Player()
{
	for(int i = 0; i < MAX_STATES; i++)
	{
		animationList[i] = NULL;
	}

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
	//cout << animationList[UP]->startRow << " " << animationList[DOWN]->startRow << endl;
	//setState(IDLE);
	if(myKey[KEY_W])
	{
		setState(UP);
		sf_walk = true;
		translateObject(0, 4, 0);
	}

	if(myKey[KEY_S])
	{
		setState(DOWN);
		sf_walk = true;
		translateObject(0, -4, 0);
	}	

	if(myKey[KEY_A])
	{
		setState(LEFT);
		sf_walk = true;
		translateObject(-4, 0, 0);
	}

	if(myKey[KEY_D])
	{
		setState(RIGHT);
		sf_walk = true;
		translateObject(4, 0, 0);
	}

	if(!myKey[KEY_W] && !myKey[KEY_A] && !myKey[KEY_S] && !myKey[KEY_D])
	{
		sf_walk = false;
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

	switch(state)
	{
	case UP:
		{
			if(mesh != animationList[UP])
			{
				setMesh(animationList[UP]);
			}
			animationList[UP]->Update(dt);
			break;
		}
	case DOWN:
		{
			if(mesh != animationList[DOWN])
			{
				setMesh(animationList[DOWN]);
			}
			animationList[DOWN]->Update(dt);
			break;
		}
	case LEFT:
		{
			if(mesh != animationList[LEFT])
			{
				setMesh(animationList[LEFT]);
			}
			animationList[LEFT]->Update(dt);
			break;
		}
	case RIGHT:
		{
			if(mesh != animationList[RIGHT])
			{
				setMesh(animationList[RIGHT]);
			}
			animationList[RIGHT]->Update(dt);
			break;
		}
	case ATTACKUP:
		{
			if(mesh != animationList[ATTACKUP])
			{
				setMesh(animationList[ATTACKUP]);
			}
			animationList[ATTACKUP]->Update(dt);
			break;
		}
	case ATTACKDOWN:
		{
			if(mesh != animationList[ATTACKDOWN])
			{
				setMesh(animationList[ATTACKDOWN]);
			}
			animationList[ATTACKDOWN]->Update(dt);
			break;
		}
	case ATTACKLEFT:
		{
			if(mesh != animationList[ATTACKLEFT])
			{
				setMesh(animationList[ATTACKLEFT]);
			}
			animationList[ATTACKLEFT]->Update(dt);
			break;
		}
	case ATTACKRIGHT:
		{
			if(mesh != animationList[ATTACKRIGHT])
			{
				setMesh(animationList[ATTACKRIGHT]);
			}
			animationList[ATTACKRIGHT]->Update(dt);
			break;
		}
	};

	//Sound
	if (sf_walk == true && !sfxengine->isCurrentlyPlaying("soundfiles/step(max).ogg"))
	{
		sfx_plyr_step = sfxengine->play2D("soundfiles/step(max).ogg");
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

void Player::setState(STATES state)
{
	this->state = state;
}

Player::STATES Player::getState()
{
	return state;
}