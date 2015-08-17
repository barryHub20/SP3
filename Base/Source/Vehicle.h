#ifndef VEHICLE_H
#define VEHICLE_H
#include "BoundBox.h"
#include "Object.h"
#include "MeshList.h"

/* parent vehicle class */
class Vehicle
{
public:
	enum TYPE
	{
		BASIC,
		TANK,
		POLICE_CRUISER,
		TOTAL_TYPES,
	};

	/* constructor / destructor */
	Vehicle();
	Vehicle(const Vehicle& copy);
	virtual ~Vehicle();

	/* core */
	virtual void Init(const Vector3& pos, vector<Object*>& objectList);
	virtual void Update();

	/* Utilities */
	virtual void moveForward(const double dt);
	virtual void moveBackward(const double dt);
	virtual void turnLeft(const double dt);
	virtual void turnRight(const double dt);
	virtual void translate(float x, float y, float z){};

	/* getter setter */
protected:
	/* physical object/s */
	Object** object_array;

	/* where its facing */
	Vector3 target;

	/* type */
	TYPE type;	//for child class to set
};

/* array of predefined vehicles */
//Vehicle preDefined_Vehicles[Vehicle::TOTAL_TYPES];

#endif