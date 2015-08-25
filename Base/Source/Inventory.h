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
	const static int MAX_SLOT = 5;

	/* constructor/destructor */
	Inventory(void);
	~Inventory(void);

	/* Core */
	//param percent_widthOfScreen: how much is the scale of inventory based on percentage of 2D window width?
	//param percent_gapPerSlot: how wide the gap btw slots, based on percentage of slot scale
	//param TWOD_Window_Width: pass in 2D window width
	//param leftPercent: offset to left: 0.5 is equal dist from left and right, 0 is to extreme left, 1 is to extreme right

	void Set(float percent_widthOfScreen, float percent_gapPerSlot, float TWOD_Window_Width, float leftPercent);

	/* utilities */
	bool addItem(Item* item);
	Item* useItem();
	Item* removeItem();
	int getCurrentSlot(void);

	/* getter */
	float getStartX();
	Vector3 getSlotScale();
	float getDistBtwSlot();
private:
	InventorySlot* arrSize[MAX_SLOT];	//array of slots
	int currentSlot;	//current slot

	/* physical (not using gameObject) */
	float percent_widthOfScreen;	//how wide is the inventory
	float percent_gapPerSlot;	//how big the gap between each slot is

	//to be used in render
	Vector3 scalePerSlot;	//scale of each slot
	float distBtwSlot;		//dist between each slot
	float startX;	//start x, for the first slot
};

#endif