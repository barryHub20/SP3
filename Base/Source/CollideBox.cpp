#include "BoundBox.h"

/* constructor/destructor */
CollideBox::CollideBox()
{
}

CollideBox::~CollideBox()
{
}

/* core */
void CollideBox::Set(const Vector3& objectPos, Vector3 scale)
{
	position = objectPos;
	this->scale = scale;

	start = position - (this->scale * 0.5f);
	end = position + (this->scale * 0.5f);

	/* internal variables */
	previousPos = position;
	previousStart = start;
	previousEnd = end;
}

/* geter/setter */
Vector3 CollideBox::getScale(){return scale;}
Vector3 CollideBox::getPosition(){return position;}
