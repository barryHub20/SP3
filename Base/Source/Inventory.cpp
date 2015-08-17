#include "Inventory.h"

Item* Inventory::arrSize[SIZE];

Inventory::Inventory(void)
{
	isFull = false;
	isEmpty = false;

	currentSize = 0;
}

Inventory::~Inventory(void)
{

}

int Inventory::getCurrentSize(void)
{
	return currentSize;
}

void Inventory::addItem(Item& item)
{
	arrSize[currentSize] = &item; //store the address of items
	currentSize++;
}

bool Inventory::deleteItemID(int index)
{
	arrSize[index] = NULL;
	return true;
}

bool Inventory::deleteItemName(char* indexName)
{
	return true;
}

