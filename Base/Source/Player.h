#ifndef PLAYER_H
#define PLAYER_H
#include "GameObject.h"

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
public:
	Player();
	Player(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active);
	~Player();
	
	void Update(double dt, bool* myKey);

	void setScore(int amtScore);
	int getScore(void);

	void setHealth(double health);
	double getHealth(void);

	void setStamina(double stamina);
	double getStamina(void);

	void setDamage(double damage);
	double getDamage(void);

	void setState(STATES state);
	Player::STATES getState();

	virtual void CollisionResponse();	//player specific collision response
};

#endif