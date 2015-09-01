#include "Inventory.h"

/************************* Inventory slot *************************/
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

	currentSize++;
	SlotSize[currentSize - 1] = addItem;
	addItem->itemLooted();
	addItem->setActive(false);
	return true;
}

bool InventorySlot::deleteItem(Vector3 currentPos)
{
	if(currentSize == 0)
	{
		return false;
	}
	else
	{
		Item* ptr = SlotSize[currentSize - 1];
		ptr->translate(currentPos);
		ptr->setActive(true);
		SlotSize[currentSize - 1]->setItemFloor(true);
		SlotSize[currentSize - 1] = NULL;
		currentSize--;
		return true;
	}
	return false;
}

Item* InventorySlot::getHighest()
{
	if(currentSize == 0)
	{
		return NULL;
	}
	else
	{
		return SlotSize[currentSize - 1];
	}
}

string InventorySlot::getCurrentItemTypeName()
{
	if(currentSize == 0)
	{
		return "None";
	}
	else
	{
		return SlotSize[currentSize - 1]->getTypeName();
	}
}

void InventorySlot::ClearAll()
{
	for(int i = 0; i < currentSize; ++i)
	{
		SlotSize[i]->setActive(false);
		SlotSize[i] = NULL;
	}
	currentSize = 0;
}

int InventorySlot::getCurrentSize()
{
	return currentSize;
}

Mesh* InventorySlot::getCurrentItemMesh()
{
	if(currentSize == 0)
	{
		return NULL;
	}
	else
	{
		return SlotSize[currentSize - 1]->getMesh();
	}
}


/************************* Inventory *************************/
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

void Inventory::Set(float percent_widthOfScreen, float percent_gapPerSlot, float TWOD_Window_Width, float TWOD_Window_height, float leftPercent, float topPercent, float yToXPercentage)
{
	//calculate
	float inventory_width = percent_widthOfScreen * TWOD_Window_Width;	//width of inventory
	float slot_Width = inventory_width / MAX_SLOT;	//width per slot
	float gap_width = slot_Width * percent_gapPerSlot;	//width per gap

	scalePerSlot.Set(slot_Width - gap_width, (slot_Width - gap_width) * yToXPercentage, 1.f);	//scale
	distBtwSlot = slot_Width;	//dist from one slot to the other
	startX = (scalePerSlot.x / 2.f) - (gap_width / 2);	//starting pos for the first slot

	float distFromEdgeLeft = ((1.f - percent_widthOfScreen)) * TWOD_Window_Width;
	startX += distFromEdgeLeft * leftPercent;

	yPos = (TWOD_Window_height * topPercent) + scalePerSlot.y * 0.5f;
}

/* getter */
float Inventory::getStartX()
{
	return startX;
}

Vector3 Inventory::getSlotScale()
{
	return scalePerSlot;
}

float Inventory::getDistBtwSlot()
{
	return distBtwSlot;
}

int Inventory::getCurrentSlot(void)
{
	return currentSlot;
}

int Inventory::currentSize(int slot)
{
	return arrSize[slot]->getCurrentSize();
}

string Inventory::currentItemName(int slot)
{
	return arrSize[slot]->getCurrentItemTypeName();
}

Mesh* Inventory::currentItemMesh(int slot)
{
	return arrSize[slot]->getCurrentItemMesh();
}

bool Inventory::addItem(Item* item)
{
	if(!item->getActive())
		return false;

	bool returnVal = false;

	/* loop thru all slots */
	int counter = 0;
	for(int i = currentSlot; counter < MAX_SLOT;)
	{
		returnVal = arrSize[i]->addItem(item);

		if(returnVal)	//succsessfully added
		{
			currentSlot = i;
			return returnVal;
		}
		else	//not added
			++i;

		if(i >= MAX_SLOT && currentSlot > 0)	//if reach last slot, loop back to first slot
		{
			counter = 0;
			i = 0;
		}

		++counter;
	}
	return false;
}

Item::ITEM_TYPE Inventory::currentItemID()
{
	if(arrSize[currentSlot]->getHighest() != NULL)
	{
		return arrSize[currentSlot]->getHighest()->getItemID();
	}
	return Item::UNDEFINED;
}

bool Inventory::clearFromInventory(Item::ITEM_TYPE typeToDispose)
{
	for(int i = 0; i < MAX_SLOT; ++i)
	{
		if(arrSize[i]->getHighest() != NULL)	//if got item
		{
			if(arrSize[i]->getHighest()->getItemID() == typeToDispose)	//if match, clear all items
			{
				arrSize[i]->ClearAll();
				return true;
			}
		}
	}
	return false;
}

void Inventory::Update(double dt, bool* myKeys)
{
	if(myKeys[SCROLL_BOTTOM])
	{
		currentSlot++;

		if(currentSlot >= MAX_SLOT)
		{
			currentSlot = 0;
		}
	}
	else if(myKeys[SCROLL_TOP])
	{
		currentSlot--;
		if(currentSlot < 0)
		{
			currentSlot = MAX_SLOT - 1;
		}
	}
}

Item* Inventory::useItem()
{
	return arrSize[currentSlot]->getHighest();
}

float Inventory::getYPos()
{
	return yPos;
}

//remove the current item (highest at current slot)
bool Inventory::removeItem(Vector3 currentPos)
{
	if(arrSize[currentSlot]->getCurrentSize() == 0)
	{
		return false;
	}

	else
	{
		if(arrSize[currentSlot]->deleteItem(currentPos))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
