#include "GameObject.h"


int GameObject::objCount = 0;

GameObject::GameObject(void)
	: isActive(false)
{
	objCount++;
}

void GameObject::setInfo(Vector3 Pos, Vector3 Dir, int Speed, int Force)
{
	this->Info.setPos(Vector2(Pos.x, Pos.y));
	this->Info.setDir(Vector2(Dir.x, Dir.y));
	this->Info.setSpeed(Speed);
	this->Info.setForce(Force);
}

void GameObject::setIsActive(bool isActive)
{
	this->isActive = isActive;
}

void GameObject::setType(GO_TYPE Type)
{
	this->Type = Type;
}

int  GameObject::getObjCount()
{
	return objCount;
}

Physics GameObject::getInfo()
{
	return Info;
}

bool GameObject::getisActive()
{
	return isActive;
}

int GameObject::getType()
{
	return Type;
}

GameObject::~GameObject(void)
{
	objCount--;
}
