#pragma once
#include"Vector3.h"
#include "Vector2.h"

class Physics
{
private:
	Vector3 Pos;
	Vector3 Dir;

	int Speed;
	int Force;
	

public:

	void setPos(Vector2 Pos);
	void setDir(Vector2 Dir);
	void setForce(int Force);
	void setSpeed(int Speed);

	Vector3 getPos();
	Vector3 getDir();
	Vector3 getForce();
	int getSpeed();

	Physics(void);
	~Physics(void);
};

