#ifndef PLAYER_H
#define PLAYER_H
#include "GameObject.h"

class Player : public GameObject
{
private:	
	int score;

public:
	Player();
	Player(Vector3 Pos, Vector3 scale, Vector3 Dir, float Speed, bool active);
	~Player();
	
	void Update(double dt, bool* myKey);

	void setScore(int amtScore);
	int getScore();
};

#endif