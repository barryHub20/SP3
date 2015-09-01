#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

#include "GameObject.h"
#include "Mesh.h"
#include "Map.h"

using namespace std;

class Item : public GameObject
{
public:

	enum ITEM_TYPE
	{
		UNDEFINED = 0,
		KEY,
		KEYR,
		KEYP,
		KEYG,
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
protected:
	string typeName;	//have name so in view no need use switch to check itemID to find out name
	ITEM_TYPE itemID;
	bool onFloor;
};

/******************************* Coin *********************************/
class Coin : public Item
{
public:
	Coin(void);
	Coin(Mesh* mesh, bool onFloor);
	Coin(const Item& item);
	~Coin(void);

	/* Init */
	//set Starting pos to player pos and specify the dir
	//UP: (0, 1, 0)
	//DOWN: (0, -1, 0)
	//LEFT: (-1, 0, 0)
	//RIGHT: (1, 0, 0)
	//Called once only
	void Activate(Vector3& playerPos, float x, float y);

	/* Update coin */
	//collision check included
	void Update(double dt, vector<Map*>* currentMap);
};

#endif