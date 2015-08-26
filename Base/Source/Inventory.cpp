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

Item* InventorySlot::deleteItem()
{
	if(currentSize == 0)
	{
		return NULL;
	}
	else
	{
		Item* ptr = SlotSize[currentSize - 1];
		ptr->setActive(true);
		SlotSize[currentSize - 1]->setItemFloor(true);
		SlotSize[currentSize - 1] = NULL;
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

void Inventory::Set(float percent_widthOfScreen, float percent_gapPerSlot, float TWOD_Window_Width, float leftPercent)
{
	this->percent_widthOfScreen = percent_widthOfScreen;
	this->percent_gapPerSlot = percent_gapPerSlot;

	//calculate
	float inventory_width = percent_widthOfScreen * TWOD_Window_Width;	//width of inventory
	float slot_Width = inventory_width / MAX_SLOT;	//width per slot
	float gap_width = slot_Width * percent_gapPerSlot;	//width per gap

	scalePerSlot.Set(slot_Width - gap_width, slot_Width - gap_width, 1.f);	//scale
	distBtwSlot = slot_Width;	//dist from one slot to the other
	startX = (scalePerSlot.x / 2.f) - (gap_width / 2);	//starting pos for the first slot

	float distFromEdgeLeft = ((1.f - percent_widthOfScreen)) * TWOD_Window_Width;
	startX += distFromEdgeLeft * leftPercent;	
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
			//cout << arrSize[i]->getCurrentSize() << ' ' << currentSlot << endl;
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
