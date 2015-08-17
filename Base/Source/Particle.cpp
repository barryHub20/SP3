#include "Particle.h"

/* static varibales */
const float Particle::m_InitialY = 500.f;
const  Vector3 Particle::m_gravity(0, -9.8, 0);

/* constructor/destructor */
Particle::Particle(){}
Particle::Particle(TYPE type){this->type = type;}
Particle::~Particle(){}

/* update */
void Particle::Set(string name, Mesh* mesh, Vector3 scale, Vector3 position, Object* parent, bool light, TYPE type, Vector3 camPos, float y_Vel)
{
	initialPos = camPos;	//camera position
	this->type = type;
	vel.Set(0, y_Vel, 0);
	Object::Set(name, mesh, scale, position, parent, light, false);
}

void Particle::Update(const double& dt, Vector3& playerPos)
{
	Object::translateObject(0, vel.y * m_gravity.y * dt, 0);

	//once reach ground: instantly reset from top again
	if(Object::position.y <= 0.f)
		Activate(playerPos);
}

void Particle::Activate(Vector3& playerPos)
{
	Vector3 offset = playerPos - initialPos;
	Object::translate(position.x + offset.x, m_InitialY, position.z + offset.z);
	initialPos = playerPos;
}

/* getter setter */
void Particle::SetPos(float x, float z)
{
	Object::position.x = x;
	Object::position.z = z;
}
