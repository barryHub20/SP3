#include "Item.h"

Item::Item(void)
{
	itemID = KEY;
	onFloor = false;
}

Item::Item(Mesh* mesh, ITEM_TYPE itemID, bool onFloor, Vector3 Pos, Vector3 scale)
{
	this->itemID = itemID;
	this->onFloor = onFloor;
	typeName = "";

	/* set object */
	Set("items", mesh, NULL, false, false);
	translateObject(Pos.x, Pos.y, Pos.z);
	scaleObject(scale.x, scale.y, scale.z);

	/* set name */
	switch (itemID)
	{
	case NOTE:
		typeName = "Note";
		break;
	case H_POTION:
		typeName = "Health";
		break;
	case S_POTION:
		typeName = "Staminia";
		break;
	case ATK_POTION:
		typeName = "Attack";
		break;
	case KEY:
		typeName = "Key";
		break;
	default:
		typeName = "Undefined";
		break;
	}

	/* set boundbox */
	collideBound.Set(Pos, scale, Collision::BOX);
}

Item::Item(const Item& item) : itemID(item.itemID)
{
}

Item::~Item(void)
{
}

void Item::setItemID(ITEM_TYPE newItemID)
{
	this->itemID = newItemID;
}

Item::ITEM_TYPE Item::getItemID(void)
{
	return itemID;
}

void Item::setItemFloor(bool onFloor)
{
	this->onFloor = onFloor;
}

bool Item::getItemFloor(void)
{
	return onFloor;
}

void Item::itemLooted(void)
{
	onFloor = false;
}

string Item::getTypeName()
{
	return typeName;
}


/******************************* Coin *********************************/
Coin::Coin(void)
{
}

Coin::Coin(Mesh* mesh, bool onFloor)
{
	this->itemID = Item::ATK_POTION;
	this->onFloor = onFloor;
	typeName = "Coin";

	Set("items", mesh, NULL, false, false);
	translateObject(0, 0, 0);
	scaleObject(50, 50, 1);

	/* set boundbox */
	collideBound.Set(position, scale, Collision::BOX);

	setActive(false);
}

Coin::Coin(const Item& item)
{
}

Coin::~Coin(void)
{
}

/* Init */
//set Starting pos to player pos and specify the dir
void Coin::Activate(Vector3& playerPos, float x, float y)
{
	//getForce()
	info.setDir(Vector2(x, y));	//direction
	info.setSpeed(20);
	info.setTimer(7);	//15 seconds to despawn

	//Set to starting position
	translate(playerPos.x, playerPos.y, 2);
	setActive(true);

	cout << position << ' ' << scale << endl;
}

/* Update coin */
//collision check included
void Coin::Update(double dt, vector<Map*>* currentMap)
{
	/* Update pos */
	translateObject(info.getForce());
	info.setSpeed(info.getSpeed() - 200 * dt);

	/* Update timer */
	info.setTimer(info.getTimer() - dt);

	/* If lifetime run out */
	if(info.getTimer() <= 0)
	{
		setActive(false);
		info.setSpeed(0);
	}

	/* Collision check */
	collideBound.Start(position);

	/* check collision with map (WALL) */
	for (int i = 0; i < currentMap->size(); i++)
	{
		if ((*currentMap)[i]->getMapType() == Map::COLLISIONMAP)
		{
			if((*currentMap)[i]->CheckCollisionWith(this))	//check this coin with the map
			{
				/* 1) If collide wall */
				onFloor = true;
				info.setSpeed(0);
			}
		}
	}
	
	/* Reset collision */
	collideBound.Reset();

	/* Response: translate pos to just outside wall if collide */
	CollisionResponse();

	/* 2) if speed <= 0 */
	if(info.getSpeed() <= 0)
	{
		onFloor = true;
		info.setSpeed(0);
	}
}

