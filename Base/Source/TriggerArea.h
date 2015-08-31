#ifndef TRIGGER_AREA_H
#define TRIGGER_AREA_H 
#include "Collision.h"

/***
	TriggerArea for general use
***/
class TriggerArea : public Collision
{
private:
	int activateCount;	//how many times must activate to be considered "triggered"
	int activateCounter;	//counts the number of times trigger
	bool activated;
public:
	/* constructor/destructor */
	TriggerArea();
	~TriggerArea();

	/* Core */
	void Set(Vector3 pos, Vector3 scale, int activateCount);
	void Reset();

	/* Getter/setter */
	int getActivateCount();
	bool getActivated();

	/* Utilities */
	//pass in player/npc collide box and whether player/npc has activate triggerBox
	void QuickAABBDetection(Collision* checkMe, bool activateOnce);	
};

#endif

