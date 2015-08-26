#include "TriggerObject.h"

TriggerObject::TriggerObject()
{

}

TriggerObject::~TriggerObject()
{

}

TriggerObject::TriggerObject(Mesh* mesh, TRIGGEROBJECTS objectName, Vector3 Pos, Vector3 scale, float angle, bool active, SoundManager &sfx_mano, Player* player)
{
	/* set object */
	Set("checking", mesh, NULL, false, false);
	translateObject(Pos.x, Pos.y, Pos.z);
	scaleObject(scale.x, scale.y, scale.z);
	type = objectName;

	/* set boundbox */
	collideBound.Set(Pos, scale, Collision::BOX);

	this->player = player;

	isTriggered = false;
	checkTriggered = false;
	triggerTimer = 0;
}

void TriggerObject::setState(TRIGGEROBJECTS state)
{
	this->type = state;
}

TriggerObject::TRIGGEROBJECTS TriggerObject::getState()
{
	return type;
}

void TriggerObject::Update(double dt, bool* myKey)
{
	FireTrap(dt, myKey);
	if(type == FIRE)
	{
		setState(FIRE);
		if (animationList[FIRE]->ended == true)
		{
			animationList[FIRE]->Reset();
		}

		switch(type)
		{
		case FIRE:
			if(mesh != animationList[FIRE])
			{
				setMesh(animationList[FIRE]);
			}
			animationList[FIRE]->Update(dt);
			break;
		};
	}
}

void TriggerObject::FireTrap(double dt, bool* myKey)
{
	triggerTimer += dt;
	if(type == NOTTRIGGERED)
	{
		if(triggerTimer > 2)
		{
			if(player->QuickAABBDetection(this) && myKey[KEY_E] && checkTriggered == false)
			{
				isTriggered = true;
				checkTriggered = true;
				triggerTimer = 0;
			}

			else if(player->QuickAABBDetection(this) && myKey[KEY_E] && checkTriggered == true)
			{
				isTriggered = false;
				checkTriggered = false;
				triggerTimer = 0;
			}
		}
	}
	cout << "TIMER: " << triggerTimer << endl << endl;
}

void TriggerObject::setDetectionBound()
{
	DetectionBound.Set(this->position, this->scale * 3, Collision::BOX);
}

Collision TriggerObject::getDetectionBound()
{
	return DetectionBound;
}

void TriggerObject::CollisionResponse()
{
	if(collided)
	{
		translate(collideBound.position);
	}
}

void TriggerObject::setTriggered(bool isTriggered)
{
	this->isTriggered = isTriggered;
}

bool TriggerObject::getTriggered(void)
{
	return isTriggered;
}