#include "UI_Object.h"

/* constructor/destructor */
UI_Object::UI_Object()
{
	message = "";
	type = TOTAL_TYPE;
}

UI_Object::~UI_Object()
{
}

/* core */
void UI_Object::Init(Mesh* mesh, Vector3 pos, Vector3 scale, string word, UI_TYPE type, bool active)
{
	/* create object */
	/* set object */
	Set("sdfdf", mesh, NULL, false, false);
	translateObject(pos.x, pos.y, pos.z);
	scaleObject(scale.x, scale.y, scale.z);
	Object::active = active;

	/* set boundbox */
	collideBound.Set(pos, scale, Collision::BOX);

	if(word.length() > 0)
		message = word;
	this->type = type;
}

void UI_Object::SetActive(bool active)
{
	this->active = active;
}

void UI_Object::Follow(float x, float y)
{
	Object::translate(x, y, position.z);
}

bool UI_Object::CheckCollision(UI_Object& check)
{
	return CollisionCheck(&check);
}

void UI_Object::CollisionResponse()
{
}

