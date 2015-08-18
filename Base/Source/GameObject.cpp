#include "GameObject.h"
#include "Controller.h"

int GameObject::objCount = 0;

GameObject::GameObject(void)
{
	objCount++;
}

GameObject::GameObject(Vector3 Pos, Vector3 scale, Vector3 Dir, float Speed, bool active)
{
	/* Set physics */
	this->info.setDir(Vector2(1, 0));
	this->info.setSpeed(Speed);

	/* Set object */
	this->object.Set("dfsdf", Geometry::meshList[Geometry::GEO_CUBE], NULL, false, false);
	this->object.translateObject(Pos);	//set pos
	this->object.scaleObject(scale.x, scale.y, scale.z);	//set scale
	this->object.setActive(active); //set active

	/* Set collision (FUTURE) */
}

void GameObject::setInfo(float Speed)
{
	/* Set physics */
	this->info.setDir(Vector2(1, 0));	//x Dir
	this->info.setSpeed(Speed);
}

Object* GameObject::getObject()
{
	return &object;
}

void GameObject::Update()
{
	float velPlayer = 750.f;	//tmp only

	if(Controller::KeyPressed(KEY_W))
	{
		object.translateObject(Vector3(0, 10, 0));
	}

	if(Controller::KeyPressed(KEY_A))	
	{
		object.translateObject(Vector3(-1, 0, 0));
	}

	if(Controller::KeyPressed(KEY_S))	
	{
		object.translateObject(Vector3(0, -1, 0));
	}

	if(Controller::KeyPressed(KEY_D))	
	{
		object.translateObject(Vector3(1, 0, 0));
	}
}

void GameObject::setIsActive(bool isActive)
{
	object.setActive(isActive);
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
	return info;
}

bool GameObject::getisActive()
{
	return object.getActive();
}

int GameObject::getType()
{
	return Type;
}

GameObject::~GameObject(void)
{
	objCount--;
}
