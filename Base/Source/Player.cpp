#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

/* core */
void Player::Set(Mesh* mesh, Vector3 scale, Vector3 position, float angle, Vector3 axis, float mass, Object* parent, bool light, int healthPoint, Gun::TYPE initType)
{
	/* character */
	Character::Set(mesh, scale, position, angle, axis, mass, parent, light, healthPoint, initType);

	/* camera */
	camera.Init(position, Vector3(position.x + 10, position.y, position.z), Vector3(0, 1, 0));

	/* gun */
	Gun* gun_ptr;
	for(int i = 0; i < Gun::TOTAL_TYPE; ++i)
	{
		//push back all types of gun
		gun_ptr = new Gun(static_cast<Gun::TYPE>(i));
		armoury.push_back(gun_ptr);
		gun_ptr->Set(Vector3(0, 0, 0));	//set gun with pos from character
	}

	/* score */
	score = 0;

	//DO NOT USE ACCESS VECTOR DIRECTLY
	currentGun = armoury[Gun::SNIPER];	//current gun is smg
	currentGun->setActive(true);
}	

void Player::Update(bool keyPressedFire, bool keyPressedReload, bool* myKeys, const double dt)
{
	static float recoilAngle = 10.f;
	static float recoilCal = 0.f;

	//recoil
	if(recoilCal > 0.f)
		recoilCal -= 100.f * dt;
	if(recoilCal < 0.f)
		recoilCal = 0.f;

	/* reset object */

		object.Reset();

	if(currentGun->getActive())
		currentGun->getObject()->Reset();

		/* camera */
		camera.Update(dt, myKeys);

		//position translate with camera
		//object.translate(camera.position);

		//update gun

	if(currentGun->getActive())
	{
		currentGun->Update(keyPressedFire, keyPressedReload, dt);

		//update gun pos
		if(currentGun->getFireBullet())
			recoilCal = recoilAngle;

		//currentGun->getObject()->translate(camera.position);
		Vector3 view = (camera.target - camera.position).Normalized();
		//Vector3 right = view.Cross(camera.up);
		//right.y = 0;
		Vector3 right = view.Cross(camera.up);
		right.y = 0;
		right.Normalize();

		float bitch = Math::RadianToDegree(asin(view.y)); 
		float angle = Math::RadianToDegree(atan2(view.x, view.z));
		angle = -(angle - 90.f);
		//modelStack.Translate(cam->target.x, cam->target.y, cam->target.z);

		//right.Normalize();
		//std::cout << angle << std::endl;
		//currentGun->getObject()->translate(view * 10);
		currentGun->getObject()->rotateObject(recoilCal, right.x, right.y, right.z);
		currentGun->getObject()->rotateObject(bitch, right.x, right.y, right.z);//pitch rotation
		currentGun->getObject()->rotateObject(-angle, 0, 1, 0);	//yaw rotation

		//switch gun


		//if hit, lose hp

		//if die, state = DIE

		/* update object */
		currentGun->getObject()->Update();
	}
	object.Update();
}

void Player::changeWeapons(bool goDown, bool goUp)
{
	static bool scrollDown = false;
	static bool scrollUp = false;
	if(goDown && !scrollDown)
	{
		scrollDown = true;
		currentGun->setActive(false);

		if(currentGun->getType() <= Gun::SNIPER)	//first weapon
			currentGun = armoury[Gun::SMG];	//set to last weapon
		else
			currentGun = armoury[currentGun->getType() - 1];

		currentGun->setActive(true);
	}
	else if(!goDown && scrollDown)
		scrollDown = false;

	if(goUp && !scrollUp)
	{
		scrollUp = true;
		currentGun->setActive(false);

		if(currentGun->getType() >= Gun::SMG)	//first weapon
			currentGun = armoury[Gun::SNIPER];	//set to last weapon
		else
			currentGun = armoury[currentGun->getType() + 1];

		currentGun->setActive(true);
	}
	else if(!goUp && scrollUp)
		scrollUp = false;
}

void Player::increaseScore(int amt)
{
	score += amt;
}

int Player::getScore()
{
	return score;
}