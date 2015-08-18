#include "Physics.h"


Physics::Physics(void) :
	Speed(0)
{

}

void Physics::setDir(Vector2 Dir)
{
	this->Dir.x = Dir.x;
	this->Dir.y = Dir.y;
}

void Physics::setSpeed(float Speed)
{
	this->Speed = Speed;
}

Vector3 Physics::getDir()
{
	return Dir;
}

float Physics::getSpeed()
{
	return Speed;
}

Vector3 Physics::getForce()
{
	return Dir * Speed;
}

Physics::~Physics(void)
{
}
