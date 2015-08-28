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
		NONE = 0,
		FIRETRIGGER, //fire lever
		ARROWTRIGGER, //arrow lever
		DOOR,
		FIRE, //fire trap
		ARROWTRAP, //arrow trap
		TRIGGERWHENCOLLIDE,
		TOTAL_OBJECTS,
	};

	TriggerObject();
	TriggerObject(Mesh* mesh, TRIGGEROBJECTS ObjectName, Vector3 Pos, Vector3 scale, float angle, bool active, SoundManager &sfx_mano, Player* player);
	~TriggerObject();

	double triggerTimer;

	void setState(TRIGGEROBJECTS state);
	TriggerObject::TRIGGEROBJECTS getState();

	void Update(double dt, bool* myKey);

	void setDetectionBound();
	Collision getDetectionBound();
	virtual void CollisionResponse();	//player specific collision response

	void setTriggered(bool isTriggered);
	bool getTriggered(void);
	void updateTrigger(double dt, bool* myKey);
private:	
	// Sound
	SoundManager *my_sfx_man;

	Player* player;

	Collision DetectionBound;
	TRIGGEROBJECTS type;

	bool isTriggered;
	Vector3 initialPos;
};

#endif