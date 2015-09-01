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
	this->active = active; //render

	/* set boundbox */
	collideBound.Set(Pos, scale, Collision::BOX);

	this->player = player;

	isTriggered = true; //trigger
	triggerTimer = 0;
	initialPos = Pos;
	speed = 10.f;
	arrowCooldown = 1.f;

	my_sfx_man = &sfx_mano;
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
			if(this->getActive() == true)
			{
				my_sfx_man->play_fire();
			}
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
	arrowCooldown += dt;

	if(type == FIRETRIGGER)
	{
		if(triggerTimer > 2)
		{
			if(player->QuickAABBDetection(this) && myKey[KEY_E] && isTriggered == true) //Switch off fire trap
			{
				isTriggered = false;
				triggerTimer = 0;
				my_sfx_man->stop_fire();
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
		if (this->mesh->name == "arrow left" && this->isTriggered == true)
		{
			translateObject(Vector3(-speed, 0, 0));
		}
		else if (this->mesh->name == "arrow right" && this->isTriggered == true)
		{
			translateObject(Vector3(speed, 0, 0));
		}
		else if (this->mesh->name == "arrow up" && this->isTriggered == true)
		{
			translateObject(Vector3(0, speed, 0));
		}
		else if (this->mesh->name == "arrow down" && this->isTriggered == true)
		{
			translateObject(Vector3(0, -speed, 0));
		}
	}
	else if (type == ARROWTRIGGER)
	{
		if (player->QuickAABBDetection(this) && isTriggered == false) //Switch on arrow trap
		{
			isTriggered = true;
			my_sfx_man->play_stone_move();
		}
	}
	else if (type == SPIKEREAPPEAR)
	{
		if (triggerTimer > duration)
		{
			if (active == true)
			{
				active = false;
				isTriggered = false;
				triggerTimer = 0.f;
				my_sfx_man->play_spike();
			}
			else if (active == false)
			{
				active = true;
				isTriggered = true;
				triggerTimer = 0.f;
				my_sfx_man->play_spike();
			}
		}
	}
}

void TriggerObject::resetPosition()
{
	Object::translate(initialPos.x, initialPos.y, initialPos.z);
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