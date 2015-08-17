#ifndef BULLETINFO_H
#define BULLETINFO_H
#include "Vector3.h"

class BulletInfo
{
private:
	bool m_status;
	Vector3 dir, pos;
	float speed, lifetime;
public:
	BulletInfo(){}
	~BulletInfo(){}

	void Init(Vector3 pos, Vector3 dir, float speed, float lifetime)
	{
		this->pos = pos;
		this->dir = dir;
		this->speed = speed;
		this->lifetime = lifetime;
		SetStatus(false);
	}

	/* getter setter */
	bool GetStatus(){return m_status;}
	void SetStatus(const bool m_status){this->m_status = m_status;};

	Vector3 getPos(){return pos;}
	void setPos(const Vector3 pos){this->pos = pos;}

	Vector3 getDir(){return dir;}
	void setDir(const Vector3 dir){this->dir = dir;}

	float GetSpeed(){return speed;}
	void SetSpeed(const float speed){this->speed = speed;}

	float GetLifetime(){return lifetime;}
	void SetLifetime(const float lifetime){this->lifetime = lifetime;}

	/* update */
	void update(const double dt)
	{
		if(m_status)
		{
			pos += dir * speed * dt;	//update pos
			lifetime -= dt;

			if(lifetime <= 0.f)
				Init(Vector3(0, 0, 0), Vector3(0, 0, 0), speed, 5.f);
				
		}
	}
};

#endif