#include "Guard.h"


Guard::Guard(void)
{
}

Guard::Guard(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active)
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
}

void Guard::StartCollisionCheck()
{
	GameObject::StartCollisionCheck();
	DetectionBound.Start(position);
}

void Guard::CollisionResponse()
{
	collideBound.Reset();
	DetectionBound.Reset();

	//response
	if(detectedPlayer == true && getState() != ES_ALERT && getState() != ES_SCAN)
	{
		setState(ES_ALERT);
		info.setTimer(2);
	}
}

void Guard::Update(float dt, vector<Map*>* level_map, vector<GameObject*>& goList)
{


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
}

void Guard::UpdateStateResponse(vector<Map*>* level_map, GameObject* Player)
{
	
	switch (this->getState())
	{
	case ES_WALK_DOWN:
	{
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
	case ES_SCAN:
	{
		cout<<"Scanning"<<endl;

			if(info.getTimer() < 0 && detectedPlayer == true)
			{
				cout<<"hi,i see you!"<<endl;
				Cell* EnemyPos = new Cell(this->getPosition().x ,this->getPosition().y);
				Cell* PlayerPos = new Cell(Player->getPosition().x,Player->getPosition().y);

				//this->PathFinder.FindPath(EnemyPos, PlayerPos, level_map);
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
	case ES_ALERT:
	{
		cout<<"I think i saw something!"<<endl;
		if (info.getTimer() < 0)
		{
			this->setState(ES_SCAN);
			info.setTimer(3);
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

void Guard::setDetecionBound()
{
	DetectionBound.Set(this->position, this->scale * 3, Collision::BOX);
}

Collision Guard::getDetecionBound()
{
	return DetectionBound;
}

Guard::~Guard(void)
{
}
