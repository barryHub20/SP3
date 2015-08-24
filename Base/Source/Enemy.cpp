#include "Enemy.h"


Enemy::Enemy(void)

{
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

void Enemy::setName(string Name)
{
	this->Name = Name;
}

string Enemy::getName()
{
	return Name;
}

void Enemy::Update(float dt, MapManager *mapManager, vector<GameObject*> goList)
{

	info.setTimer(info.getTimer()-dt);
	
	GameObject* Player;

			for(int i = 0; i < goList.size();++i)
			{
				if(goList[i]->getType() == GameObject::GO_PLAYER)
				{
					Player = goList[i];
				}
			}

	UpdateStateResponse(mapManager,Player);
}

void Enemy::setDestinationReached(bool DestinationReached)
{
	this->DestinationReached = DestinationReached;
}

bool Enemy::getDestinationReached()
{
	return DestinationReached;
}

void Enemy::UpdateStateResponse(MapManager *mapManager, GameObject* Player)
{
 	
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
