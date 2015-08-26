#ifndef TRIGGEROBJECT_H
#define TRIGGEROBJECT_H
#include "GameObject.h"
#include "Inventory.h"
#include "Player.h"

class TriggerObject : public GameObject
{
public:
	enum TRIGGEROBJECTS
	{
		ISTRIGGERED = 0,
		NOTTRIGGERED,
		DOOR,
		FIRE,
		TRIGGERWHENCOLLIDE,
		TOTAL_OBJECTS,
	};

	TriggerObject();
	TriggerObject(Mesh* mesh, TRIGGEROBJECTS ObjectName, Vector3 Pos, Vector3 scale, float angle, bool active, SoundManager &sfx_mano);
	~TriggerObject();

	void setState(TRIGGEROBJECTS state);
	TriggerObject::TRIGGEROBJECTS getState();

	void Update(double dt, bool* myKey);

	void setDetectionBound();
	Collision getDetectionBound();

	virtual void CollisionResponse();	//player specific collision response
private:	
	// Sound
	SoundManager *my_sfx_man;
	Collision DetectionBound;
	TRIGGEROBJECTS type;
};

#endif