#include "Inventory.h"

Item Inventory::arrSize[MAX_SIZE];

Inventory::Inventory(void)
{
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
	arrSize[currentSize] = item; //store the address of items
	currentSize++;
}

bool Inventory::deleteItem(int index)
{
	Item* tempArrSize = new Item[currentSize];

	for(int i = 0; i < currentSize; i++)
	{
		tempArrSize[i] = arrSize[i];
	}

	int find = 0;

	for(int i = 0; i < currentSize; i++)
	{
		if(arrSize[i].getItemID() == index)
		{
			find = i;
			std::cout << std::endl;
			std::cout << "This item will be deleted." << std::endl << std::endl;
		}
	}

	for(int i = find; i < currentSize; i++)
	{
		//Replace
		arrSize[i] = tempArrSize[i + 1];
	}

	currentSize--;

	if(currentSize < 0)
	{
		currentSize = 0;
	}

	delete []tempArrSize;
	return true;
}
