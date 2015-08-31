#include "TriggerArea.h"

/* constructor/destructor */
TriggerArea::TriggerArea()
{
	activated = false;
	activateCounter = 0;
}

TriggerArea::~TriggerArea()
{
}

/* Core */
void TriggerArea::Set(Vector3 pos, Vector3 scale, int activateCount)
{
	position = pos;
	this->scale = scale;
	this->type = Collision::BOX;
	this->activateCount = activateCount;
}

void TriggerArea::Reset()
{
	activateCounter = 0;
	activated = false;
}

/* Getter/setter */
int TriggerArea::getActivateCount()
{
	return activateCounter;
}

bool TriggerArea::getActivated()
{
	return activated;
}

/* Utilities */
void TriggerArea::QuickAABBDetection(Collision* checkMe, bool activateOnce)
{
	/* If collide */
	if(Collision::QuickAABBDetection(checkMe) && activateOnce)
	{
		activateCounter++;
	}

	/* if reached activation count */
	if(activateCounter >= activateCount)
	{
		activated = true;	//this trigger box is now activated
	}
}