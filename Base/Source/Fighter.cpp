#include "Fighter.h"

/* constructor / destrutor */
Fighter::Fighter()
{
}

Fighter::~Fighter()
{
}

/* core */
void Fighter::Set(Mesh* mesh, Vector3 scale, Vector3 position, float angle, Vector3 axis, float mass, Object* parent, bool light, int healthPoint, Gun::TYPE initType)
{
	/* character */
	Character::Set(mesh, scale, position, angle, axis, mass, parent, light, healthPoint, initType);

	/* fighter */
	fighterPos = position;
	initialPos = position;
	this->angleFacing = angle;
	targetAngle = angle;

	waypoint1.Set(-3000, 200, 0);

	speed = 200.f;

	despawnTime = 60;
	despawnTimer = 0;
}

void Fighter::Update(const double dt)		//Controller::myKeys
{
	object.Reset();

	if(despawnTimer < despawnTime)
	{
		state = PATROL;
		despawnTimer += dt;
	}
	if(despawnTimer >= despawnTime || healthPoint <= 0)
	{
		fighterPos = initialPos;
		healthPoint = 20;
		despawnTimer = 0;
		state = DIE;
	}

	Vector3 view = (waypoint1 - fighterPos).Normalized();

	fighterPos += (view * speed * dt);
	//object.translate(fighterPos);

	//if(object.getPosition().z > waypoint1.z)
	//	object.setPosition(object.getPosition() + (view * speed * dt));

	object.Update();
}

Vector3 Fighter::getPos()
{
	return fighterPos;
}