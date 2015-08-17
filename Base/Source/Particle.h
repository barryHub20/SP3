#ifndef PARTICLE_H
#define PARTICLE_H
#include "Object.h"
#include "Billboard.h"

class Particle : public Object
{
public:
	enum TYPE
	{
		SNOW,
		SLEET,
		TOTAL_TYPE,
	};

	/* static variables */
	const static Vector3 m_gravity;
	const static float m_InitialY;

	/* constructor/destructor */
	Particle();
	Particle(TYPE type);
	~Particle();

	/* update */
	void Set(string name, Mesh* mesh, Vector3 scale, Vector3 position, Object* parent, bool light, TYPE type, Vector3 camPos, float y_Vel);
	virtual void Update(const double& dt, Vector3& playerPos);
	void Activate(Vector3& playerPos);

	/* getter setter */
	void SetPos(float x, float z);	//set x and z; y predetermined in Set()

private:
	TYPE type;
	Vector3 initialPos;
	Vector3 vel;
};

#endif