#include "Inventory.h"

InventorySlot::InventorySlot(void)
{
	currentSize = 0;
	currentType = Item::UNDEFINED;

	for(int i = 0; i < MAX_SIZE; ++i)
	{
		SlotSize[i] = NULL;
	}
}

InventorySlot::~InventorySlot(void)
{

}


bool InventorySlot::addItem(Item* addItem)
{
	//If empty, item type will be undefined

	if(currentSize == 0)
	{
		currentType = addItem->getItemID();
	}

	else if(currentSize > 0 && currentSize < MAX_SIZE)
	{
		if(currentType != addItem->getItemID())
		{
			return false;
		}
	}

	else if(currentSize >= MAX_SIZE)
	{
		return false;
	}

	SlotSize[currentSize] = addItem;
	addItem->itemLooted();
	addItem->setActive(false);
	currentSize++;

	return true;
}

Item* InventorySlot::deleteItem()
{
	if(currentSize == 0)
	{
		return NULL;
	}
	else
	{
		Item* ptr = SlotSize[currentSize];
		ptr->setActive(true);
		SlotSize[currentSize]->setItemFloor(true);
		SlotSize[currentSize] = NULL;
		currentSize--;
		return ptr;
	}
}

Item* InventorySlot::getHighest()
{
	if(currentSize == 0)
	{
		return NULL;
	}
	else
	{
		return SlotSize[currentSize];
	}
}

int InventorySlot::getCurrentSize()
{
	return currentSize;
}

Inventory::Inventory(void)
{
	currentSlot = 0;

	for(int i = 0; i < MAX_SLOT; ++i)
	{
		arrSize[i] = new InventorySlot;
	}
}

Inventory::~Inventory(void)
{

}

int Inventory::getCurrentSlot(void)
{
	return currentSlot;
}

bool Inventory::addItem(Item* item)
{
	bool returnVal = false;

	/* loop thru all slots */
	int counter = 0;
	for(int i = currentSlot; counter < MAX_SLOT;++i)
	{
		returnVal = arrSize[i]->addItem(item);

		if(returnVal)	//successfully added
		{
			currentSlot = i;
			return returnVal;
		}
		else	//not added
			++i;

		if(i >= MAX_SLOT)	//if reach last slot, loop back to first slot
			i = 0;

		++counter;
	}
	return false;
}

Item* Inventory::useItem()
{
	return arrSize[currentSlot]->getHighest();
}

Item* Inventory::removeItem()
{
	if(arrSize[currentSlot]->getCurrentSize() == 0)
	{
		return NULL;
	}

	else
	{
		return arrSize[currentSlot]->deleteItem();
	}
}
