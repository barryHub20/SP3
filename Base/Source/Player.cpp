#include "Player.h"
#include "Application.h"
#include "LoadTGA.h"

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
	PLAYER_SPEED = 4;
	score = 0;
	health = 100;
	stamina = 60;
	damage = 10;

	/* set object */
	Set("sdfdf", mesh, NULL, false, false);
	translateObject(Pos.x, Pos.y, Pos.z);
	scaleObject(scale.x, scale.y, scale.z);

	/* set angle */
	angleZ = angle;

	/* set physics */
	info.setSpeed(Speed);
	info.setDir(Vector2(1, 0));	//should be based on angle
	info.setTimer(0);

	/* set boundbox */
	collideBound.Set(Pos, scale, Collision::BOX);
	invisible = false;

	jumpSpeed = 0;

	setType(GO_PLAYER);
	// Sound setup
	my_sfx_man = &sfx_mano;

	deceleration = 15;
	vel = 0;
	LeftOrRight = false;
	UpOrDown = false;
	checkLR = false;
	checkUD = false;

	/* drop rate */
	dropRate = 0.1;
	dropTimer = dropRate;

	/* Store all non-invisibility sprites */
	//Player sprites
	Sprite_invisibility_texture_file_path = "Image//Sprites//Hero2_invisible.tga";

	storeSpriteAnimation("black guard", 21, 13, "Image//Sprites//Hero2.tga");
	processSpriteAnimation(Player::UP, 0.5f, 0, 8, 8, 8, 1);
	processSpriteAnimation(Player::DOWN, 0.5f, 0, 10, 8, 10, 1);
	processSpriteAnimation(Player::LEFT, 0.5f, 0, 9, 8, 9, 1);
	processSpriteAnimation(Player::RIGHT, 0.5f, 0, 11, 8, 11, 1);
	processSpriteAnimation(Player::ATTACKUP, 0.5f, 0, 4, 7, 4, 1);
	processSpriteAnimation(Player::ATTACKDOWN, 0.5f, 0, 6, 7, 6, 1);
	processSpriteAnimation(Player::ATTACKLEFT, 0.5f, 0, 5, 7, 5, 1);
	processSpriteAnimation(Player::ATTACKRIGHT, 0.5f, 0, 7, 7, 7, 1);

	/* Set coin list */
	for(int i = 0; i < 20; ++i)
	{
		Coin* coin;
		coin = new Coin(Geometry::meshList[Geometry::GEO_COIN], false);
		coinList.push_back(coin);
	}

	throwTime = 0.5f;
	throwTimer = throwTime;
}

void Player::switchInvisibleState()
{
	const char* txt;
	if(invisible)
	{
		invisible = false;
		txt = Sprite_texture_file_path;
	}
	else
	{
		invisible = true;
		txt = Sprite_invisibility_texture_file_path;
	}

	animationList[UP]->textureID[0] = LoadTGA(txt);
	animationList[DOWN]->textureID[0] = LoadTGA(txt);
	animationList[LEFT]->textureID[0] = LoadTGA(txt);
	animationList[RIGHT]->textureID[0] = LoadTGA(txt);
	animationList[ATTACKUP]->textureID[0] = LoadTGA(txt);
	animationList[ATTACKDOWN]->textureID[0] = LoadTGA(txt);
	animationList[ATTACKLEFT]->textureID[0] = LoadTGA(txt);
	animationList[ATTACKRIGHT]->textureID[0] = LoadTGA(txt);
}

Player::~Player()
{
}

void Player::throwCoin(double dt, bool throwKey)
{
	/* Set coin list */
	if(throwTimer < throwTime)
		throwTimer += dt;
	else
	{
		if(throwKey)
		{
			throwTimer = 0.0;
	
			for(int i = 0; i < 20; ++i)
			{
				/* If not active, activate */
				if(!coinList[i]->getActive())
				{
					if(state == UP || state == IDLE)
						coinList[i]->Activate(position, 0, 1);
					else if(state == LEFT)
						coinList[i]->Activate(position, -1, 0);
					else if(state == RIGHT)
						coinList[i]->Activate(position, 1, 0);
					else if(state == DOWN)
						coinList[i]->Activate(position, 0, -1);
					break;
				}
			}
		}
	}
}

void Player::Update(double dt, bool* myKey)
{
	Vector3 Pos;

	// Counts from 0
	info.setTimer(info.getTimer() + dt);

	/* update inventory */
	inventory.Update(dt, myKey);

	/* Use item */
	useItem(myKey);

	//cout << animationList[UP]->startRow << " " << animationList[DOWN]->startRow << endl;
	//setState(IDLE);
	if(myKey[KEY_W])
	{
		UpOrDown = false;
		checkUD = false;
		if(vel.y < PLAYER_SPEED)
		{
			vel.y += 0.5;
			Pos.y = vel.y;
		}
		else
		{
			vel.y = PLAYER_SPEED;
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

	else if(myKey[KEY_S])
	{
		UpOrDown = true;
		checkUD = true;
		if(vel.y > -PLAYER_SPEED)
		{
			vel.y -= 0.5;
			Pos.y = vel.y;
		}
		else
		{
			vel.y = -PLAYER_SPEED;
			Pos.y = vel.y;
		}

		setState(DOWN);
		if (animationList[DOWN]->ended == true)
		{
			animationList[DOWN]->Reset();
		}

		sf_walk = true;
	}	


	else if(myKey[KEY_A])
	{
		LeftOrRight = true;
		checkLR = true;
		if(vel.x >= -PLAYER_SPEED)
		{
			vel.x -= 0.5;
			Pos.x = vel.x;
		}
		else
		{
			vel.x = -PLAYER_SPEED;
			Pos.x = vel.x;
		}
		
		setState(LEFT);
		if (animationList[LEFT]->ended == true)
		{
			animationList[LEFT]->Reset();
		}
		sf_walk = true;
	}

	else if(myKey[KEY_D])
	{
		LeftOrRight = false;
		checkLR = false;
		if(vel.x <= PLAYER_SPEED)
		{
			vel.x += 0.5;
			Pos.x = vel.x;
		}
		else
		{
			vel.x = PLAYER_SPEED;
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

bool Player::getInvisible()
{
	return invisible;
}

void Player::setInvisible(bool b)
{
	invisible = b;
}

bool Player::pickUp(Item* item, bool* myKey)
{
	if(myKey[KEY_E])
	{
		if(QuickAABBDetection(item))	//if player collide with item
		{
			if(inventory.addItem(item))
			{
				cout << "SUCCESSFUL" << endl;
				my_sfx_man->play_pickup();
				return true;
			}
			else
			{
				cout << "NO SUCCESSFUL" << endl;
				return false;
			}
		}
	}
	return false;
}

bool Player::dropItem(double dt, Item* item, bool* myKey)
{
	if(dropTimer < dropRate)
	{
		dropTimer += dt;
	}
	else
	{
		dropTimer = 0.0;

		if(myKey[KEY_O])
		{
			if(item->getItemID() == Item::NOTE)	//notes are non disposable
				return false;

			if(inventory.removeItem(this->position))
			{
				cout << "DROP SUCCESSFUL" << endl;
				my_sfx_man->play_drop();
				return true;
			}

			else
			{
				cout << "NONONONONO" << endl;
				return false;
			}
		}
	}
	return false;
}

bool Player::useItem(bool* myKey)
{	
	Item* ptr = inventory.useItem();

	if(ptr == NULL)
	{
		return false;
	}
	
	if(info.getTimer() > 0.5)
	{
		if(myKey[KEY_L] && health < 100 && ptr->getItemID() == Item::H_POTION)
		{
			if(inventory.removeItem(this->position))
			{
				ptr->setActive(false);
			}
			setHealth(getHealth() + 10);
			if(getHealth() >= 100)
			{
				health = 100;
			}
			my_sfx_man->play_yummy();
			info.setTimer(0);
		}

		else if(myKey[KEY_L] && stamina < 100 && ptr->getItemID() == Item::S_POTION)
		{
			if(inventory.removeItem(this->position))
			{
				ptr->setActive(false);
			}
			setStamina(getStamina() + 20);
			if(getStamina() >= 100)
			{
				stamina = 100;
			}
			my_sfx_man->play_yummy();
			info.setTimer(0);
		}
	}
	return false;
}

Inventory* Player::getInventory()
{
	return &inventory;
}