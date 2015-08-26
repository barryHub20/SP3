#include "TriggerObject.h"

TriggerObject::TriggerObject()
{

}

TriggerObject::~TriggerObject()
{

}

TriggerObject::TriggerObject(Mesh* mesh, TRIGGEROBJECTS objectName, Vector3 Pos, Vector3 scale, float angle, bool active, SoundManager &sfx_mano)
{
	/* set object */
	Set("checking", mesh, NULL, false, false);
	translateObject(Pos.x, Pos.y, Pos.z);
	scaleObject(scale.x, scale.y, scale.z);
	type = objectName;

	/* set boundbox */
	collideBound.Set(Pos, scale, Collision::BOX);
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