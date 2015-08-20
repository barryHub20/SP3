#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"

class Inventory : public Item
{
private:
	const static int MAX_SIZE = 6;
	static Item arrSize[MAX_SIZE];
	int currentSize;
	 
public:
	Inventory(void);
	~Inventory(void);

	void addItem(Item& item);
	bool deleteItem(int index);

	int getCurrentSize(void);
};
#endif