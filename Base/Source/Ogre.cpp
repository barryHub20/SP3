#include "Ogre.h"


Ogre::Ogre(void)
{
}

Ogre::Ogre(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active)
{
	/* set object */
	Set("enemy", mesh, NULL, false, false);
	translateObject(Pos.x, Pos.y, Pos.z);
	scaleObject(scale.x, scale.y, scale.z);

	/* set angle */
	angleZ = angle;

	/* set physics */
	srand(time(NULL));
	info.setSpeed(Speed);
	info.setDir(Vector2(1, 0));	//should be based on angle

	/* set boundbox */
	collideBound.Set(Pos, scale, Collision::BOX);

	/* set detection box */
	DetectionBound.Set(Pos, scale * 5, Collision::BOX);

	/*PathFind Variable*/
	RouteToTake = 0;
	Route = Vector3(0,0,0);

	bool isDestReached = true;
	bool isChasing = false;


	/* Enemy Variables */
	this->setState(ES_IDLE);
	this->setType(GO_ENEMY);
	info.setTimer(5);
	this->setName("Ogre");
	this->setHealth(100);
	detectedPlayer = false;
	collided = false;
	this->setDestinationReached(false);
	this->setDestination(Vector2(400, 400));

	storeSpriteAnimation("ogre", 21, 13, "Image//Sprites//orc.tga");
	processSpriteAnimation(ES_WALK_UP, 0.5f, 0, 8, 8, 8, 1);
	processSpriteAnimation(ES_WALK_DOWN, 0.5f, 0, 10, 8, 10, 1);
	processSpriteAnimation(ES_WALK_LEFT, 0.5f, 0, 9, 8, 9, 1);
	processSpriteAnimation(ES_WALK_RIGHT, 0.5f, 0, 11, 8, 11, 1);
	processSpriteAnimation(ES_ATTACK_UP, 0.5f, 0, 4, 7, 4, 1);
	processSpriteAnimation(ES_ATTACK_DOWN, 0.5f, 0, 6, 7, 6, 1);
	processSpriteAnimation(ES_ATTACK_LEFT, 0.5f, 0, 5, 7, 5, 1);
	processSpriteAnimation(ES_ATTACK_RIGHT, 0.5f, 0, 7, 7, 7, 1);
}

void Ogre::StartCollisionCheck()
{
	GameObject::StartCollisionCheck();
	DetectionBound.Start(position);
}

void Ogre::CollisionResponse()
{
	collideBound.Reset();
	DetectionBound.Reset();

	//response
	if(detectedPlayer == true && getState() != ES_ALERT && getState() != ES_SCAN && getState() != ES_CHASE && getState() != ES_ATTACK)
	{
		setState(ES_ALERT);
		info.setTimer(2);
	}
}

void Ogre::Update(float dt, vector<Map*>* level_map, vector<GameObject*>& goList)
{
	//cout<<Route;

	info.setTimer(info.getTimer() - dt);

	for (int i = 0; i < goList.size(); ++i)
	{
		if (goList[i]->getType() == GameObject::GO_PLAYER)
		{
			UpdateStateResponse(level_map, goList[i]);
		}
	}

	StartCollisionCheck();


	//check
	for(int i = 0; i < goList.size();++i)
	{
		if(goList[i]->getType() == GameObject::GO_PLAYER)
		{
			detectedPlayer = Collision::CheckCollision(this->DetectionBound, *(goList[i]->getCollideBound()));
		}

	}

	//reset
	CollisionResponse();

	//Sprite animation

	//Sprite animation
	switch (this->getState())
	{
	case ES_WALK_UP:
	{
		if (mesh != animationList[ES_WALK_UP])
		{
			setMesh(animationList[ES_WALK_UP]);
		}
		animationList[ES_WALK_UP]->Update(dt);
		break;
	}
	case ES_WALK_DOWN:
	{
		if (mesh != animationList[ES_WALK_DOWN])
		{
			setMesh(animationList[ES_WALK_DOWN]);
		}
		animationList[ES_WALK_DOWN]->Update(dt);
		break;
	}
	case ES_WALK_LEFT:
	{
		if (mesh != animationList[ES_WALK_LEFT])
		{
			setMesh(animationList[ES_WALK_LEFT]);
		}
		animationList[ES_WALK_LEFT]->Update(dt);
		break;
	}
	case ES_WALK_RIGHT:
	{
		if (mesh != animationList[ES_WALK_RIGHT])
		{
			setMesh(animationList[ES_WALK_RIGHT]);
		}
		animationList[ES_WALK_RIGHT]->Update(dt);
		break;
	}
	case ES_CHASE:
	{
		switch (lastState)
		{
		case ES_ATTACK_UP:
		{
			if (mesh != animationList[ES_ATTACK_UP])
			{
				setMesh(animationList[ES_ATTACK_UP]);
			}
			animationList[ES_ATTACK_UP]->Update(dt);
			break;
		}
		case ES_ATTACK_DOWN:
		{
			if (mesh != animationList[ES_ATTACK_DOWN])
			{
				setMesh(animationList[ES_ATTACK_DOWN]);
			}
			animationList[ES_ATTACK_DOWN]->Update(dt);
			break;
		}
		case ES_ATTACK_LEFT:
		{
			if (mesh != animationList[ES_ATTACK_LEFT])
			{
				setMesh(animationList[ES_ATTACK_LEFT]);
			}
			animationList[ES_ATTACK_LEFT]->Update(dt);
			break;
		}
		case ES_ATTACK_RIGHT:
		{
			if (mesh != animationList[ES_ATTACK_RIGHT])
			{
				setMesh(animationList[ES_ATTACK_RIGHT]);
			}
			animationList[ES_ATTACK_RIGHT]->Update(dt);
			break;
		}
		}
		break;
	
	}
	case ES_ATTACK:
	{
		switch(lastState)
		{
		case ES_ATTACK_UP:
		{
			if (mesh != animationList[ES_ATTACK_UP])
			{
				setMesh(animationList[ES_ATTACK_UP]);
			}
			animationList[ES_ATTACK_UP]->Update(dt);
			break;
		}
		case ES_ATTACK_DOWN:
		{
			if (mesh != animationList[ES_ATTACK_DOWN])
			{
				setMesh(animationList[ES_ATTACK_DOWN]);
			}
			animationList[ES_ATTACK_DOWN]->Update(dt);
			break;
		}
		case ES_ATTACK_LEFT:
		{
			if (mesh != animationList[ES_ATTACK_LEFT])
			{
				setMesh(animationList[ES_ATTACK_LEFT]);
			}
			animationList[ES_ATTACK_LEFT]->Update(dt);
			break;
		}
		case ES_ATTACK_RIGHT:
		{
			if (mesh != animationList[ES_ATTACK_RIGHT])
			{
				setMesh(animationList[ES_ATTACK_RIGHT]);
			}
			animationList[ES_ATTACK_RIGHT]->Update(dt);
			break;
		}
		}
		break;
	}
	};
}

void Ogre::UpdateStateResponse(vector<Map*>* level_map, GameObject* player)
{
	switch (this->getState())
	{
	case ES_WALK_DOWN:
	{
		if (animationList[ES_WALK_DOWN]->ended == true)
		{
			animationList[ES_WALK_DOWN]->Reset();
		}

		for (int i = 0; i < level_map->size(); i++)
		{
			if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
			{
				if (info.getTimer() < 0 || (*level_map)[i]->getWalkable(getPosition().x, getPosition().y - 50) == false || (*level_map)[i]->getWalkable(getPosition().x - 25, getPosition().y - 50) == false || (*level_map)[i]->getWalkable(getPosition().x + 25, getPosition().y - 50) == false || (*level_map)[i]->getWalkable(getPosition().x - 25, getPosition().y - 50) == false)
				{
					this->setState(ES_IDLE);
				}
			}
		}
		translateObject(0, -1, 0);

		break;
	}
	case ES_WALK_UP:
	{
		if (animationList[ES_WALK_UP]->ended == true)
		{
			animationList[ES_WALK_UP]->Reset();
		}
		for (int i = 0; i < level_map->size(); i++)
		{
			if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
			{
				if (info.getTimer() < 0 || (*level_map)[i]->getWalkable(getPosition().x, getPosition().y + 50) == false || (*level_map)[i]->getWalkable(getPosition().x + 25, getPosition().y + 50) == false || (*level_map)[i]->getWalkable(getPosition().x - 25, getPosition().y + 50) == false)
				{
					this->setState(ES_IDLE);
				}
			}
		}
		translateObject(0, 1, 0);

		break;
	}
	case ES_WALK_RIGHT:
	{
		if (animationList[ES_WALK_RIGHT]->ended == true)
		{
			animationList[ES_WALK_RIGHT]->Reset();
		}
		for (int i = 0; i < level_map->size(); i++)
		{
			if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
			{
				if (info.getTimer() < 0 || (*level_map)[i]->getWalkable(getPosition().x + 50, getPosition().y) == false || (*level_map)[i]->getWalkable(getPosition().x + 50, getPosition().y + 25) == false || (*level_map)[i]->getWalkable(getPosition().x + 50, getPosition().y - 25) == false)
				{
					this->setState(ES_IDLE);
				}
			}
		}
		translateObject(1, 0, 0);

		break;
	}
	case ES_WALK_LEFT:
	{
		if (animationList[ES_WALK_LEFT]->ended == true)
		{
			animationList[ES_WALK_LEFT]->Reset();
		}
		for (int i = 0; i < level_map->size(); i++)
		{
			if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
			{
				if (info.getTimer() < 0 || (*level_map)[i]->getWalkable(getPosition().x - 50, getPosition().y) == false || (*level_map)[i]->getWalkable(getPosition().x - 50, getPosition().y + 25) == false || (*level_map)[i]->getWalkable(getPosition().x - 50, getPosition().y - 25) == false)
				{
					this->setState(ES_IDLE);
				}
			}
		}
		translateObject(-1, 0, 0);

		break;
	}
	case ES_IDLE:
	{
		int MovementState = Math::RandIntMinMax(1, 4);
		int Timer = Math::RandIntMinMax(1, 10);

		switch (MovementState)
		{
		case 1:
		{
			this->setState(ES_WALK_UP);
			info.setTimer(Timer);
			break;
		}
		case 2:
		{
			this->setState(ES_WALK_DOWN);
			info.setTimer(Timer);
			break;
		}

		case 3:
		{
			this->setState(ES_WALK_RIGHT);
			info.setTimer(Timer);
			break;
		}
		case 4:
		{
			this->setState(ES_WALK_LEFT);
			info.setTimer(Timer);
			break;
		}
		default:
		{
			this->setState(ES_IDLE);
		}
		}
		break;
	}
	case ES_CHASE:
		{
			cout<<"Chasing"<<endl;

			Cell* EnemyPos = new Cell(this->getPosition().x ,this->getPosition().y);
			Cell* PlayerPos = new Cell(player->getPosition().x,player->getPosition().y);

			RouteList = this->PathFinder.FindPath(EnemyPos, PlayerPos, level_map); // returns a vector of Cell
			Route = Vector3(RouteList[RouteToTake]->getTileX()*32,RouteList[RouteToTake]->getTileY()*32); // Route is a vector3 

			if(((this->getPosition() - player->getPosition()).Length() / 32) > 10)
			{
				setState(ES_IDLE);
				info.setTimer(1);
				cout << "GOING TO IDLE NOW" << endl;
			}
			cout<<static_cast<int>((this->getPosition() - player->getPosition()).Length() / 32)<<endl;
			if(static_cast<int>((this->getPosition() - player->getPosition()).Length() / 32) < 2)
			{
				setState(ES_ATTACK);
			}

			info.setTimer(1); 
			
			isDestReached = false;
		}

		if(this->getPosition() != Route)
		{
			if(this->getPosition().x == Route.x)
			{
				if(this->getPosition().y > Route.y )
				{
					lastState = ES_ATTACK_DOWN;
					translateObject(0, -1, 0);
				}
				if(this->getPosition().y < Route.y )
				{
					lastState = ES_ATTACK_UP;
					translateObject(0, 1, 0);
				}
			}
			else
			{
				if(this->getPosition().x > Route.x )
				{
					lastState = ES_ATTACK_LEFT;
					translateObject(-1, 0, 0);
				}
				if(this->getPosition().x < Route.x )
				{
					lastState = ES_ATTACK_RIGHT;
					translateObject(1, 0, 0);
				}

			}
			//Sprite animation when chasing
			switch (lastState)
			{
			case ES_ATTACK_UP:
			{
				if (animationList[ES_ATTACK_UP]->ended == true)
				{
					animationList[ES_ATTACK_UP]->Reset();
				}
				break;
			}
			case ES_ATTACK_DOWN:
			{
				if (animationList[ES_ATTACK_DOWN]->ended == true)
				{
					animationList[ES_ATTACK_DOWN]->Reset();
				}
				break;
			}
			case ES_ATTACK_LEFT:
			{
				if (animationList[ES_ATTACK_LEFT]->ended == true)
				{
					animationList[ES_ATTACK_LEFT]->Reset();
				}
				break;
			}
			case ES_ATTACK_RIGHT:
			{
				if (animationList[ES_ATTACK_RIGHT]->ended == true)
				{
					animationList[ES_ATTACK_RIGHT]->Reset();
				}
				break;
			}
			}
		}
		else
		{
			RouteToTake +=1;
			if(RouteToTake == RouteList.size())
			{
				isDestReached = true;
			}
			break;
		}
	case ES_SCAN:
	{
		cout<<"Scanning"<<endl;

			if(info.getTimer() < 0 && detectedPlayer == true)
			{
				cout<<"hi,i see you!"<<endl;
			
				setState(ES_CHASE);
				//info.setTimer(1);
				break;
			}
			if(info.getTimer() < 0 && detectedPlayer == false)
			{
				cout<<"guess im imagining tings!"<<endl;
				setState(ES_IDLE);
				break;	
			}
			break;	
	}	

	case ES_ATTACK:
		{
			switch (lastState)
			{
			case ES_ATTACK_UP:
			{
				if (animationList[ES_ATTACK_UP]->ended == true)
				{
					animationList[ES_ATTACK_UP]->Reset();
				}
				break;
			}
			case ES_ATTACK_DOWN:
			{
				if (animationList[ES_ATTACK_DOWN]->ended == true)
				{
					animationList[ES_ATTACK_DOWN]->Reset();
				}
				break;
			}
			case ES_ATTACK_LEFT:
			{
				if (animationList[ES_ATTACK_LEFT]->ended == true)
				{
					animationList[ES_ATTACK_LEFT]->Reset();
				}
				break;
			}
			case ES_ATTACK_RIGHT:
			{
				if (animationList[ES_ATTACK_RIGHT]->ended == true)
				{
					animationList[ES_ATTACK_RIGHT]->Reset();
				}
				break;
			}
			}
			
			cout << "IM GONNA ATTACK NOW" << endl;
			if(((this->getPosition() - player->getPosition()).Length() / 32) > 5)
			{
				cout << "CHASE" << endl;
				setState(ES_CHASE);
				break;
			}
			break;
		}
	case ES_ALERT:
	{
		cout<<"I think i saw something!"<<endl;
		if (info.getTimer() < 0)
		{
			this->setState(ES_SCAN);
			info.setTimer(1);
		}
		break;
	}
	case ES_ESCAPE:
	{
		break;
	}
	default:
	{
		break;
	}

	}
}

void Ogre::setDetecionBound()
{
	DetectionBound.Set(this->position, this->scale * 3, Collision::BOX);
}

Collision Ogre::getDetecionBound()
{
	return DetectionBound;
}

Ogre::~Ogre(void)
{
}
