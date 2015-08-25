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

	/* Enemy Variables */
	this->setState(ES_ALERT);
	this->setType(GO_ENEMY);
	info.setTimer(5);
	this->setName("Ogre");
	this->setHealth(100);
	this->setDestinationReached(false);
	this->setDestination(Vector2(400, 400));
}

void Ogre::Update(float dt, MapManager *mapManager, vector<GameObject*> goList)
{


	info.setTimer(info.getTimer() - dt);

	for (int i = 0; i < goList.size(); ++i)
	{
		if (goList[i]->getType() == GameObject::GO_PLAYER)
		{
			UpdateStateResponse(mapManager, goList[i]);
		}
	}

}

void Ogre::UpdateStateResponse(MapManager *mapManager, GameObject* Player)
{
	switch (this->getState())
	{
	case ES_WALK_DOWN:
	{
		for (int i = 0; i < mapManager->GetCurrentMap()->size(); i++)
		{
			if ((*mapManager->GetCurrentMap())[i]->getMapType() == Map::COLLISIONMAP)
			{
				if (info.getTimer() < 0 || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x, getPosition().y - 50) == false || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x - 25, getPosition().y - 50) == false || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x + 25, getPosition().y - 50) == false || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x - 25, getPosition().y - 50) == false)
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
		for (int i = 0; i < mapManager->GetCurrentMap()->size(); i++)
		{
			if ((*mapManager->GetCurrentMap())[i]->getMapType() == Map::COLLISIONMAP)
			{
				if (info.getTimer() < 0 || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x, getPosition().y + 50) == false || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x + 25, getPosition().y + 50) == false || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x - 25, getPosition().y + 50) == false)
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
		for (int i = 0; i < mapManager->GetCurrentMap()->size(); i++)
		{
			if ((*mapManager->GetCurrentMap())[i]->getMapType() == Map::COLLISIONMAP)
			{
				if (info.getTimer() < 0 || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x + 50, getPosition().y) == false || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x + 50, getPosition().y + 25) == false || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x + 50, getPosition().y - 25) == false)
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
		for (int i = 0; i < mapManager->GetCurrentMap()->size(); i++)
		{
			if ((*mapManager->GetCurrentMap())[i]->getMapType() == Map::COLLISIONMAP)
			{
				if (info.getTimer() < 0 || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x - 50, getPosition().y) == false || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x - 50, getPosition().y + 25) == false || (*mapManager->GetCurrentMap())[i]->getWalkable(getPosition().x - 50, getPosition().y - 25) == false)
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
			//cout<<"Scanning"<<endl;
			//cout<<this->CollisionCheck(Player);
			//if(info.getTimer() < 0 && this->CollisionCheck(Player) == true)
			//{
			//
			//}
			//break;
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
		//cout<<"Scanning"<<endl;
		//cout<<this->CollisionCheck(Player);
		if (info.getTimer() < 0 && this->CollisionCheck(Player) == true)
		{

		}
		break;
	}
	case ES_ALERT:
	{
		//cout<<"I think i saw something!"<<endl;
		if (info.getTimer() < 0)
		{
			this->setState(ES_SCAN);
			info.setTimer(5);
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
