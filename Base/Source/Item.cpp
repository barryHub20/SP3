#include "Item.h"

Item::Item()
{

}

Item::~Item()
{

}

void Item::Set(const char newName[], const int newID)
{
	for(int i = 0; i < 32; i++)
	{
		this->itemName[i] = newName[i];
	}

	this->itemID = newID;
}

void Item::setName(char newName[])
{
	for(int i = 0; i < 32; i++)
	{
		this->itemName[i] = newName[i];
	}
}

char* Item::getName(void)
{
	return itemName;
}

void Item::setID(int newID)
{
	this->itemID = newID;
}

int Item::getID(void)
{
	return itemID;
}