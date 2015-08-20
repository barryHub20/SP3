#include "Item.h"

Item::Item(void) : itemID()
{
}

Item::Item(int itemID)
{
	this->itemID = itemID;
}

Item::Item(const Item& item) : itemID(item.itemID)
{
}

Item::~Item(void)
{
}

void Item::setItemID(int newItemID)
{
	this->itemID = newItemID;
}

int Item::getItemID(void)
{
	return itemID;
}