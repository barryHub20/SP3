#pragma once
#include"Vector3.h"
#include "Vector2.h"



class Physics
{
private:
	Vector3 Dir;
	float Speed;
	double timer;	//general timer

public:
	void setDir(Vector2 Dir);
	void setSpeed(float Speed);

	Vector3 getDir();
	Vector3 getForce();
	float getSpeed();

	Physics(void);
	~Physics(void);
};

