#ifndef UI_OBJECT_H
#define UI_OBJECT_H
#include "GameObject.h"

class UI_Object : public GameObject
{
public:
	enum UI_TYPE
	{
		MOUSE_CURSOR,
		BUTTON,
		INVENTORY_SLOT,
		HP_BAR,
		MAIN_MENU_BACKGROUND,
		TOTAL_TYPE,
	};

	/* constructor/destructor */
	UI_Object();
	~UI_Object();

	/* core */
	void Init(Mesh* mesh, Vector3 pos, Vector3 scale, string word, UI_TYPE type, bool active);	//put empty for no message
	void Follow(float x, float y);	//follow this pos, eg. mouse cursor

	/* Utilities */
	bool CheckCollision(UI_Object& check);
	virtual void CollisionResponse();
	void SetActive(bool active);

private:
	UI_TYPE type;
	string message;
};


#endif