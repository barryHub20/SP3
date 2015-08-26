#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

#include "GameObject.h"
#include "Mesh.h"

using namespace std;

class Item : public GameObject
{
public:

	enum ITEM_TYPE
	{
		UNDEFINED = 0,
		KEY,
		NOTE,
		H_POTION,
		S_POTION,
		ATK_POTION,
		TOTAL_TYPE,
	};

	Item(void);
	Item(Mesh* mesh, ITEM_TYPE itemID, bool onFloor, Vector3 Pos, Vector3 scale);
	Item(const Item& item);
	~Item(void);

	void setItemID(ITEM_TYPE newItemID);
	ITEM_TYPE getItemID(void);

	void setItemFloor(bool onFloor);
	bool getItemFloor(void);

	void itemLooted(void);

	string getTypeName();
private:
	string typeName;	//have name so in view no need use switch to check itemID to find out name
	ITEM_TYPE itemID;
	bool onFloor;
};

#endif