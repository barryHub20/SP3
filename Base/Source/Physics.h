#pragma once
#include"Vector3.h"
#include "Vector2.h"



class Physics
{
private:
	Vector3 Dir;
	float Speed;
	float Timer;	//general timer

public:
	void setDir(Vector2 Dir);
	void setSpeed(float Speed);
	void setTimer( float Timer);

	Vector3 getDir();
	Vector3 getForce();
	float getSpeed();
	float getTimer();

	Physics(void);
	~Physics(void);
};

