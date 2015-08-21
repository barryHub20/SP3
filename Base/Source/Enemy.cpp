#include "Enemy.h"


Enemy::Enemy(void)
	: State(ES_IDLE)
	, Name("Default")
	,Health(100)
	,DestinationReached(false)
{
}

Enemy::Enemy(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active)
{	
	/* set object */
	Set("enemy", mesh, NULL, false, false);
	translateObject(Pos.x, Pos.y, Pos.z);
	scaleObject(scale.x, scale.y, scale.z);

	/* set angle */
	angleZ = angle;

	/* set physics */
	info.setSpeed(Speed);
	info.setDir(Vector2(1, 0));	//should be based on angle

	/* set boundbox */
	collideBound.Set(Pos, scale, Collision::BOX);
	
	/* Enemy Variables */
	State = ES_IDLE;
	Name = "Overloaded Constructor";
	Health = 100;
	DestinationReached = false;
	srand(time(NULL));
	setDestination(Vector2(400,400));
}

void Enemy::setState(ENEMY_STATE State)
{
	this->State = State;
}

void Enemy::setHealth(int Health)
{
	this->Health = Health;
}

int Enemy::getHealth()
{
	return Health;
}

int Enemy::getState()
{
	return State;
}

void Enemy::setName(string name)
{
	this->Name = Name;
}

string Enemy::getName()
{
	return Name;
}

void Enemy::Update(float dt, MapManager *mapManager)
{
	cout<<info.getTimer()<<" "<< State <<endl;
	info.setTimer(info.getTimer()-dt);

	UpdateStateResponse(mapManager);
}

void Enemy::setDestinationReached(bool DestinationReached)
{
	this->DestinationReached = DestinationReached;
}

bool Enemy::getDestinationReached()
{
	return DestinationReached;
}

void Enemy::UpdateStateResponse(MapManager *mapManager)
{
 	switch(State)
	{
	case ES_WALK_DOWN:
		{
			if(info.getTimer() < 0  || mapManager->GetCurrentMap()->getWalkable(getPosition().x,getPosition().y-50) == false || mapManager->GetCurrentMap()->getWalkable(getPosition().x-25,getPosition().y-50) == false  || mapManager->GetCurrentMap()->getWalkable(getPosition().x+25,getPosition().y-50) == false								 || mapManager->GetCurrentMap()->getWalkable(getPosition().x-25,getPosition().y-50) == false)
			{
				State = ES_IDLE;
			}
			translateObject(0,-1,0);
			
			break;
		}
	case ES_WALK_UP:
		{
			if(info.getTimer() < 0 || mapManager->GetCurrentMap()->getWalkable(getPosition().x,getPosition().y+50) == false || mapManager->GetCurrentMap()->getWalkable(getPosition().x+25,getPosition().y+50) == false  || mapManager->GetCurrentMap()->getWalkable(getPosition().x-25,getPosition().y+50) == false)
			{
				State = ES_IDLE;
			}
			translateObject(0,1,0);
			
			break;
		}
	case ES_WALK_RIGHT:
		{
			if(info.getTimer() < 0 || mapManager->GetCurrentMap()->getWalkable(getPosition().x+50,getPosition().y) == false || mapManager->GetCurrentMap()->getWalkable(getPosition().x+50,getPosition().y+25) == false  || mapManager->GetCurrentMap()->getWalkable(getPosition().x+50,getPosition().y-25) == false)
			{
				State = ES_IDLE;
			}
			translateObject(1,0,0);
			
			break;
		}
	case ES_WALK_LEFT:
		{
			
			if(info.getTimer() < 0 || mapManager->GetCurrentMap()->getWalkable(getPosition().x-50,getPosition().y) == false || mapManager->GetCurrentMap()->getWalkable(getPosition().x-50,getPosition().y+25) == false || mapManager->GetCurrentMap()->getWalkable(getPosition().x-50,getPosition().y-25) == false)
			{
				State = ES_IDLE;
			}
			translateObject(-1,0,0);

			break;
		}
	case ES_IDLE:
		{
			int MovementState = Math::RandIntMinMax(1,4);
			int Timer = Math::RandIntMinMax(1,10);

			switch(MovementState)
			{
			case 1:
				{
					State = ES_WALK_UP;
					info.setTimer(Timer);
					break;
				}
			case 2:
				{
					State = ES_WALK_DOWN;
					info.setTimer(Timer);
					break;
				}

			case 3:
				{
					State = ES_WALK_RIGHT;
					info.setTimer(Timer);
					break;
				}
			case 4:
				{
					State = ES_WALK_LEFT;
					info.setTimer(Timer);
					break;
				}
			default:
				{
					State = ES_IDLE;
				}
			}
			break;	
		}
	case ES_SCAN:
		{

			break;	
		}
	case ES_ALERT:
		{
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

void Enemy::setDestination(Vector2 Destination)
{
	this->Destination = Destination;
}

Vector2 Enemy::getDestination()
{
	return Destination;
}

Enemy::~Enemy(void)
{
}
