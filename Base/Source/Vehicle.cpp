#include "Vehicle.h"

/******************
Make vehicle an abstract class
PoliceCruiser cannot translate: ERROR: Vector iterator not incrementable
******************/

/* constructor / destructor */
Vehicle::Vehicle()
{
}

Vehicle::Vehicle(const Vehicle& copy)
{
}

Vehicle::~Vehicle()
{
}

/* core */
void Vehicle::Init(const Vector3& pos, vector<Object*>& objectList)
{
}

void Vehicle::Update()
{
}

/* utilities */
void Vehicle::moveForward(const double dt)
{
}

void Vehicle::moveBackward(const double dt)
{
}

void Vehicle::turnLeft(const double dt)
{
}

void Vehicle::turnRight(const double dt)
{
}