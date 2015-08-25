#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include "MeshList.h"

class InventorySlot
{
public:
	const static int MAX_SIZE = 5;

	InventorySlot();
	~InventorySlot();

	bool addItem(Item*);
	Item* deleteItem();
	Item* getHighest();

	int getCurrentSize();

private:
	Item* SlotSize[MAX_SIZE];
	int currentSize;
	Item::ITEM_TYPE currentType;
};

class Inventory
{
public:
	Inventory(void);
	~Inventory(void);

	const static int MAX_SLOT = 5;
	bool addItem(Item* item);
	Item* useItem();
	Item* removeItem();

	int getCurrentSlot(void);
private:
	InventorySlot* arrSize[MAX_SLOT];
	int currentSlot;
};

#endif