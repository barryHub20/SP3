#ifndef ITEM_H
#define ITEM_H

#include <iostream>

class Item
{
private:
	static const int MAX_SIZE = 32;
	int itemID;

public:
	Item(void);
	Item(int itemID);
	Item(const Item& item);
	~Item(void);

	void setItemID(int newItemID);
	int getItemID(void);
};

#endif