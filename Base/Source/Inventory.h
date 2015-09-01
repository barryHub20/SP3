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
	bool deleteItem(Vector3 currentPos);
	void ClearAll();	//USE WISELY LOL: set all items active to false
	Item* getHighest();

	int getCurrentSize();
	string getCurrentItemTypeName();	//returns none if no item
	Mesh* getCurrentItemMesh();	//returns NULL if empty
private:
	Item* SlotSize[MAX_SIZE];
	int currentSize;	//total objects inside
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

	void Set(float percent_widthOfScreen, float percent_gapPerSlot, float TWOD_Window_Width, float TWOD_Window_height, float leftPercent, float topPercent, float yToXPercentage = 1.15f);

	/* utilities */
	void Update(double dt, bool* myKeys);
	bool addItem(Item* item);
	Item* useItem();
	bool removeItem(Vector3 currentPos);

	/* getter */
	float getStartX();
	float getYPos();
	Vector3 getSlotScale();
	float getDistBtwSlot();
	int getCurrentSlot(void);
	int currentSize(int slot);	//get size of certain slot
	string currentItemName(int slot);
	Mesh* currentItemMesh(int slot);
	Item::ITEM_TYPE currentItemID();
	bool clearFromInventory(Item::ITEM_TYPE typeToDispose);	//clear all items of this type
private:
	InventorySlot* arrSize[MAX_SLOT];	//array of slots
	int currentSlot;	//current slot

	/* physical (not using gameObject) */
	//to be used in render
	Vector3 scalePerSlot;	//scale of each slot
	float distBtwSlot;		//dist between each slot
	float startX;	//start x, for the first slot
	float yPos;	//y pos for all slots
};

#endif