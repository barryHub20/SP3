#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"

class Inventory : public Item
{
private:
	const static int SIZE = 6;

	static Item* arrSize[SIZE];
	int currentSize;
	int getCount;
	bool isFull;
	bool isEmpty;

public:
	Inventory(void);
	~Inventory(void);

	void addItem(Item& item);
	bool deleteItemID(int index);
	bool deleteItemName(char* indexName);
	//bool deleteItemObj(int itemCount);

	int getCurrentSize(void);


};
#endif