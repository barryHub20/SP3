#ifndef PLAYER_H
#define PLAYER_H
#include "Character.h"

class Player : public Character
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
	
	void Update(double dt, bool* myKey, float x_limit, float y_limit);

	void setScore(int amtScore);
	int getScore();

	virtual void CollisionResponse();	//player specific collision response
};

#endif