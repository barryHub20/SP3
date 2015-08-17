#ifndef ITEM_H
#define ITEM_H

#include <iostream>

class Item
{
private:
	static const int MAX_SIZE = 32;
	char itemName[MAX_SIZE];
	int itemID;

public:
	Item();
	~Item();

	void Set(const char newName[], const int newID);

	void setName(char newName[]);
	char* getName(void);

	void setID(int newID);
	int getID(void);
};

#endif