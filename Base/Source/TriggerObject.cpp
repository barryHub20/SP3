#include "TriggerObject.h"

float ARROW_SPEED = 10.f;

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
	this->active = active; //render

	/* set boundbox */
	collideBound.Set(Pos, scale, Collision::BOX);

	this->player = player;

	isTriggered = true; //trigger
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
	updateTrigger(dt, myKey); //Actual update

	//Sprite animation
	switch (type)
	{
	case FIRE:
	{
		setState(FIRE);
		if (animationList[FIRE]->ended == true)
		{
			animationList[FIRE]->Reset();
		}

		if (mesh != animationList[FIRE])
		{
			setMesh(animationList[FIRE]);
		}
		animationList[FIRE]->Update(dt);

		break;
	}
	}
}

void TriggerObject::updateTrigger(double dt, bool* myKey)
{
	triggerTimer += dt; //timer for trigger

	if(type == FIRETRIGGER)
	{
		if(triggerTimer > 2)
		{
			if(player->QuickAABBDetection(this) && myKey[KEY_E] && isTriggered == true) //Switch off fire trap
			{
				isTriggered = false;
				triggerTimer = 0;
			}

			else if(player->QuickAABBDetection(this) && myKey[KEY_E] && isTriggered == false) //Switch on fire trap
			{
				isTriggered = true;
				triggerTimer = 0;
			}
		}
	}

	else if (type == ARROWTRAP)
	{
		if (this->mesh->name == "arrow left")
		{
			translateObject(Vector3(-ARROW_SPEED, 0, 0));
			if (position.x < 0)
			{
				position.x = 940;
			}
		}
		else if (this->mesh->name == "arrow right")
		{
			translateObject(Vector3(ARROW_SPEED, 0, 0));
			if (position.x < 0)
			{
				position.x = 940;
			}
		}
		else if (this->mesh->name == "arrow up")
		{
			translateObject(Vector3(0, ARROW_SPEED, 0));
			if (position.x < 0)
			{
				position.x = 940;
			}
		}
		else if (this->mesh->name == "arrow down")
		{
			translateObject(Vector3(0, -ARROW_SPEED, 0));
			if (position.x < 0)
			{
				position.x = 940;
			}
		}
	}
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