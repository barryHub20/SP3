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