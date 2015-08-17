#ifndef FIGHTER_H
#define FIGHTER_H
#include "Character.h"

class Fighter : public Character
{
	/* points */
	float angleFacing;
	float targetAngle;
	Vector3 waypoint1;	//pass this waypoint then rotate away
	float speed;

	Vector3 fighterPos;
	Vector3 initialPos;
	double despawnTimer;
	double despawnTime;

public:
	/* constructor / destrutor */
	Fighter();
	virtual ~Fighter();

	/* core */
	virtual void Set(Mesh* mesh, Vector3 scale, Vector3 position, float angle, Vector3 axis, float mass, Object* parent, bool light, int healthPoint, Gun::TYPE initType);
	void Update(const double dt);		//Controller::myKeys

	Vector3 getPos();
};

#endif