#include "Player.h"

Player::Player()
{
	for(int i = 0; i < MAX_STATES; i++)
	{
		animationList[i] = NULL;
	}

	health = 0;
	stamina = 0;
	damage = 0;
}

Player::Player(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active, SoundManager &sfx_mano)
{
	score = 0;
	health = 100;
	stamina = 100;
	damage = 10;

	cout << health << endl;

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

	setType(GO_PLAYER);
	// Sound setup
	my_sfx_man = &sfx_mano;

	deceleration = 10;
	vel.x = 0;
	vel.y = 0;
	LeftOrRight = false;
	UpOrDown = false;
	checkLR = false;
	checkUD = false;
}

Player::~Player()
{
}

void Player::Update(double dt, bool* myKey)
{
	Vector3 Pos;

	//cout << animationList[UP]->startRow << " " << animationList[DOWN]->startRow << endl;
	//setState(IDLE);
	if(myKey[KEY_W])
	{
		UpOrDown = false;
		checkUD = false;
		if(vel.y < 5)
		{
			vel.y += 1;
			Pos.y = vel.y;
		}
		else
		{
			vel.y = 5;
			Pos.y = vel.y;
		}

		//Movement / physics
		//translateObject(0, 4, 0);
		//Animation
		setState(UP);
		if (animationList[UP]->ended == true)
		{
			animationList[UP]->Reset();
		}

		//Sound
		sf_walk = true;
	}

	if(myKey[KEY_S])
	{
		UpOrDown = true;
		checkUD = true;
		if(vel.y > -5)
		{
			vel.y -= 1;
			Pos.y = vel.y;
		}
		else
		{
			vel.y = -5;
			Pos.y = vel.y;
		}

		setState(DOWN);
		if (animationList[DOWN]->ended == true)
		{
			animationList[DOWN]->Reset();
		}

		sf_walk = true;
	}	


	if(myKey[KEY_A])
	{
		LeftOrRight = true;
		checkLR = true;
		if(vel.x > -5)
		{
			vel.x -= 1;
			Pos.x = vel.x;
		}
		else
		{
			vel.x = -5;
			Pos.x = vel.x;
		}
		
		setState(LEFT);
		if (animationList[LEFT]->ended == true)
		{
			animationList[LEFT]->Reset();
		}
		sf_walk = true;
	}

	if(myKey[KEY_D])
	{
		LeftOrRight = false;
		checkLR = false;
		if(vel.x < 5)
		{
			vel.x += 1;
			Pos.x = vel.x;
		}
		else
		{
			vel.x = 5;
			Pos.x = vel.x;
		}
		
		setState(RIGHT);
		if (animationList[RIGHT]->ended == true)
		{
			animationList[RIGHT]->Reset();
		}
		sf_walk = true;
	}
	
	if(!myKey[KEY_W] && !myKey[KEY_S] && vel.y != 0)
	{
		if(UpOrDown == false && checkUD == false)
		{
			vel.y -= deceleration * dt;
			if(vel.y < 0 && !myKey[KEY_W])
			{
				vel.y = 0;
			}
		}

		if(UpOrDown == true && checkUD == true)
		{
			vel.y += deceleration * dt;
			if(vel.y > 0 && !myKey[KEY_S])
			{
				vel.y = 0;
			}
		}
		Pos.y += vel.y;
	}

	if(!myKey[KEY_A] && !myKey[KEY_D] && vel.x != 0)
	{
		if(LeftOrRight == false && checkLR == false)
		{
			vel.x -= deceleration * dt;
			if(vel.x < 0 && !myKey[KEY_D])
			{
				vel.x= 0;
			}
		}

		if(LeftOrRight == true && checkLR == true)
		{
			vel.x += deceleration * dt;
			if(vel.x > 0 && !myKey[KEY_A])
			{
				vel.x = 0;
			}
		}
		Pos.x += vel.x;
	}

	translateObject(Pos);

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
	if (sf_walk == true)
	{
		my_sfx_man->play_plyr_steps();
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

void Player::setHealth(double health)
{
	this->health = health;
}

double Player::getHealth(void)
{
	return health;
}

void Player::setStamina(double stamina)
{
	this->stamina = stamina;
}

double Player::getStamina(void)
{
	return stamina;
}

void Player::setDamage(double damage)
{
	this->damage = damage;
}

double Player::getDamage(void)
{
	return damage;
}

void Player::setState(STATES state)
{
	this->state = state;
}

Player::STATES Player::getState()
{
	return state;
}

bool Player::pickUp(Item* item, bool* myKey)
{
	if(myKey[KEY_E])
	{
		if(Collision::CheckCollision(collideBound, *item->getCollideBound()))	//if player collide with item
		{
			if(inventory.addItem(item))
			{
				cout << "SUCCESSFUL" << endl;
			}
			else
			{
				cout << "NO SUCCESSFUL" << endl;
			}
		}
	}
	return false;
}

bool Player::dropItem(Item* item, bool* myKey)
{
	if(myKey[KEY_O])
	{
		if(inventory.removeItem())
		{
			cout << "DROP SUCCESSFUL" << endl;
		}

		else
		{
			cout << "NONONONONO" << endl;
		}
	}
	return false;
}

bool Player::useItem(Item* item, bool* myKey)
{
	if(item->H_POTION && health < 100)
	{
		setHealth(getHealth() + 10);
		if(getHealth() >= 100)
		{
			health = 100;
		}
	}
}

Inventory* Player::getInventory()
{
	return &inventory;
}