#include "Gun.h"
#include "Model.h"

Gun::Gun()
{
	fireBullet = false;
}

Gun::Gun(TYPE type)
{
	fireBullet = false;
	this->type = type;
}

Gun::~Gun()
{
}

/* core */
void Gun::Set(Vector3& initialPos)
{
	Mesh* meshType;

	/* set gun base on type */
	switch (type)
	{
	case SNIPER:
		{
			this->fireRate = 0.5;
			this->reloadRate = 1.0;
			this->clipSize = 5;
			this->totalClip = 10;
			meshType = Model::meshList[Model::GEO_SNIPER_RIFLE];
			break;
		}
	case PISTOL:
		{
			this->fireRate = 0.2;
			this->reloadRate = 1.0;
			this->clipSize = 8;
			this->totalClip = 10;
			meshType = Model::meshList[Model::GEO_PISTOL];
			break;
		}
	case SMG:
		{
			this->fireRate = 0.1;
			this->reloadRate = 0.5;
			this->clipSize = 30;
			this->totalClip = 10;
			meshType = Model::meshList[Model::GEO_SMG];
			break;
		}
	}

	/* all same settings */
	fireTimer = fireRate;
	reloadTimer = reloadRate;
	reloading = false;
	active = false;
	currentAmmoInClip = clipSize;	//full clip
	this->totalClip -= 1;

	this->totalAmmo = (this->clipSize * this->totalClip);

	object.Set("ammo", meshType, Vector3(2, 2, 2), initialPos, NULL, false);
	object.Init();
}

void Gun::Update(bool keyPressedFire, bool keyPressedReload, const double dt)
{
	fireBullet = false;

	/* update timer */
	if(fireTimer < fireRate)
		fireTimer += dt;

	if(reloadTimer < reloadRate)
		reloadTimer += dt;

	/* reload finish */
	if(reloading && reloadTimer >= reloadRate)
	{
		/* transfer bullet */
		if(totalAmmo >= clipSize)
		{
			totalAmmo -= (clipSize - currentAmmoInClip);
			currentAmmoInClip = clipSize;
		}
		else
		{
			currentAmmoInClip = totalAmmo;
			totalAmmo = 0;
		}

		reloading = false;
	}

	/* if clip is not empty */
	if(currentAmmoInClip > 0 && !reloading)
	{
		if(fireTimer >= fireRate && keyPressedFire)
		{
			fireBullet = true;
			--currentAmmoInClip;
			fireTimer = 0.0;
		}
	}

	/* if not full can reload */
	if(keyPressedReload && currentAmmoInClip < clipSize && totalAmmo > 0)	//if player reloads
	{
		reloadTimer = 0.0;
		reloading = true;
	}
}

/* getter setter */
Object* Gun::getObject()
{
	return &object;
}

double Gun::getFireRate()
{
	return fireRate;
}

double Gun::getReloadRate()
{
	return reloadRate;
}

int Gun::getTotalAmmo()
{
	return totalAmmo;
}

int Gun::getTotalClip()
{
	return totalClip;
}

int Gun::getClipSize()
{
	return clipSize;
}

int Gun::getCurrentAmmoInClip()
{
	return currentAmmoInClip;
}

bool Gun::getFireBullet()
{
	return fireBullet;
}

bool  Gun::getReloading()
{
	return reloading;
}

Gun::TYPE Gun::getType()
{
	return type;
}

void Gun::setActive(bool b)
{
	active = b;
}

bool Gun::getActive()
{
	return active;
}