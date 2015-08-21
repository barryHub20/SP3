#pragma once
#include <ostream>
#include "Vector3.h"
#include <vector>
using namespace std;

/**
STRICTLY FOR AABB ONLY
store bool for all 6 sides of a cuboid collision box for AABB only
**/
struct Movement_3d
{
	/* For Bound Box */
	enum COLLIDE
	{
		start_X,
		end_X,
		start_Y,
		end_Y, 
		start_Z,
		end_Z,
		UNDEFINED,
	};

	/* constructor destructor */
	COLLIDE collideSide;	//which side collide
	Movement_3d(){collideSide = UNDEFINED;}
	Movement_3d(const Movement_3d& copy){collideSide = copy.collideSide;}
	~Movement_3d(){}

	/* utilities */
	void Reset(){collideSide = UNDEFINED;}

	Movement_3d& operator= (Movement_3d& copy){collideSide = copy.collideSide;return *this;}
	bool operator== (Movement_3d::COLLIDE& check){return collideSide == check;}
};


class Collision
{
public:
	enum TYPE
	{
		BOX,
		SPHERE,
		SLANTED_BOX,
	};

	/* Core */
	//initialize
	void Set(Vector3 pos, Vector3 scale, TYPE type);

	//runtime
	void Start(const Vector3& objectPos);	//before checking must set up
	void Reset();

	/* Check if collide: */
	//pass in current object's collide box and collide box of object being compared
	static bool CheckCollision(Collision& current, Collision& check);

	//1) current type: Sphere --- Check type: Sphere 
	//3) current type: Sphere --- Check type: Slanted Box
	//4) current type: Sphere --- Check type: Box
	//2) current type: Box --- Check type: Box

		/* Sphere (current) to Sphere (check) */
		static bool SphereToSphere(Collision* current, Collision* check);

		/* Sphere (current) to Slanted Box (check) */
		static bool SphereToSlantedBox(Collision* current, Collision* check);

		/* Sphere (current) to Box (check) */
		static bool SphereToBox(Collision* current, Collision* check);

		/* Box (current) to Box (check) */
		static bool BoxToBox(Collision* current, Collision* check);

	/* Collision response */
	//for respone, set object pos to collision pos
	Collision();
	~Collision();


	TYPE type;

	/* Variables for all collision type */
	Vector3 position;
	Vector3 scale;	//if is Sphere, scale.set(radius, radius, radius);

	/* aabb variables */
	Vector3 previousPos;
	Movement_3d collideArea;

private:

	/** internal functions please ignore (DO NOT CALL FROM OUTSIDE CLASS) **/
	static bool inZone(float& start, float& end, float& checkStart, float& checkEnd);
	static void getAABBCollide(float& start, float& end, float& checkStart, float& checkEnd, Movement_3d::COLLIDE startDir, Movement_3d::COLLIDE endDir, Movement_3d::COLLIDE& collideSide);
	static void UpdateAABB(Collision* current, Collision* check, Vector3& pos);
	void ResetAABB();
};
