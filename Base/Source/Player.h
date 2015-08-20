#ifndef PLAYER_H
#define PLAYER_H
#include "GameObject.h"

class Player : public GameObject
{
private:	
	int score;
	bool PlayerInAir;
	bool PlayerOnGround;
	double jumpSpeed;
	double jumpTimer;
public:
	Player();
	Player(Mesh* mesh, Vector3 Pos, Vector3 scale, float angle, float Speed, bool active);
	~Player();
	
	void Update(double dt, bool* myKey);

	void setScore(int amtScore);
	int getScore();

	virtual void CollisionResponse();	//player specific collision response
};

#endif