#ifndef GUN_H
#define GUN_H
#include "Camera3.h"
#include "Object.h"
#include <vector>
using namespace std;


class Bullet
{
};

class Gun
{
public:
	enum TYPE
	{
		SNIPER,
		PISTOL,
		SMG,
		TOTAL_TYPE,
	};
private:
	/* physical part */
	Object object;

	/* timers */
	double fireRate;
	double fireTimer;
	double reloadRate;
	double reloadTimer;

	/* ammo */
	int totalAmmo;	//move this to player class
	int totalClip;	//move this to player class
	int clipSize;	//how much ammo should a clip hold
	int currentAmmoInClip;	//currently how many bullets in gun clip

	/* flag */
	bool fireBullet;	//true if fire a bullet this frame
	bool reloading;
	bool active;

	/* type */
	TYPE type;
public:
	Gun();
	Gun(TYPE type);
	~Gun();

	/* core */
	void Set(Vector3& initialPos);	//parent is the character holding this gun
	void Update(bool keyPressedFire, bool keyPressedReload, const double dt);

	/* getter setter */
	Object* getObject();

	double getFireRate();
	double getReloadRate();

	int getTotalAmmo();	//move this to player class
	int getTotalClip();	//move this to player class
	int getClipSize();	//how much ammo should a clip hold
	int getCurrentAmmoInClip();	//currently how many bullets in gun clip

	bool getFireBullet();
	TYPE getType();
	bool getReloading();
	void setActive(bool b);
	bool getActive();
};


#endif