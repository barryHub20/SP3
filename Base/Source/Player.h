#ifndef PLAYER_H
#define PLAYER_H
#include "GameObject.h"
#include "Inventory.h"

class Player : public GameObject
{
public:
	enum STATES
	{
		IDLE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		ATTACKUP,
		ATTACKDOWN,
		ATTACKLEFT,
		ATTACKRIGHT,
		SNEAK,
		JUMP,
		MAX_STATES,
	};
private:	
	int score;
	double health;
	double stamina;
	double damage;

	bool PlayerInAir;
	bool PlayerOnGround;
	double jumpSpeed;
	double jumpTimer;
	STATES state;
	bool sf_walk;

	// Sound
	SoundManager *my_sfx_man;

	//Inventory
	Inventory inventory;
	Inventory weaponInventory;

	double dropRate;
	double dropTimer;
public:
	Player();
	Player(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active, SoundManager &sfx_mano);
	~Player();
	
	float deceleration;
	float acceleration;
	Vector3 vel;
	bool LeftOrRight;
	bool UpOrDown;
	bool checkLR;
	bool checkUD;

	void Update(double dt, bool* myKey);

	void setScore(int amtScore);
	int getScore(void);

	void setHealth(double health);
	double getHealth(void);

	void setStamina(double stamina);
	double getStamina(void);

	void setDamage(double damage);
	double getDamage(void);

	Inventory* getInventory();

	void setState(STATES state);
	Player::STATES getState();

	//add item
	bool pickUp(Item* item, bool* myKey);
	//drop item
	bool dropItem(double dt, Item* item, bool* myKey);

	bool useItem(bool* myKey);

	virtual void CollisionResponse();	//player specific collision response
};

#endif