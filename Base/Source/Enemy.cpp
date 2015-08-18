#include "Enemy.h"


Enemy::Enemy(void)
{
	setType(GO_ENEMY);
}

void Enemy::setName(string Name)
{
	this->Name = Name;
}

void Enemy::setState(ENEMY_STATE State)
{
	this->State = State;
}

string Enemy::getName(void)
{
	return Name;
}

int Enemy::getState(void)
{
	return State;
}

Enemy::~Enemy(void)
{
}
