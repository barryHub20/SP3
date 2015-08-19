#pragma once
#include <ostream>
#include "Vector3.h"
#include <vector>
using namespace std;

/**
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

	friend std::ostream& operator<< (std::ostream& os, Movement_3d& moveme){
		std::cout << moveme.collideSide;
		return os;
	}

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
	bool CheckCollision(Collision& check);

		/* if current type is sphere */
		bool CheckSphereDetection(Collision& check);

		/* if current type is AABB Box */
		bool CheckBoxDetection(Collision& check);


		bool AABB_CheckCollision(Collision& check);	//box to box aabb
		bool Sphere_To_Box_CheckCollision(Collision& check);	//sphere to box

	/* Collision response */
	//for respone, set object pos to collision pos
	Collision();
	~Collision();


	TYPE type;

	/* Variables for all collision type */
	Vector3 position;
	Vector3 scale;	//if 2D, z == 0

	/* aabb variables */
	Vector3 previousPos;
	Movement_3d collideArea;

private:

	/** internal functions please ignore (DO NOT CALL FROM OUTSIDE CLASS) **/
	bool inZone(float& start, float& end, float& checkStart, float& checkEnd);
	void getAABBCollide(float& start, float& end, float& checkStart, float& checkEnd, Movement_3d::COLLIDE startDir, Movement_3d::COLLIDE endDir, Movement_3d::COLLIDE& collideSide);
	void UpdateAABB(Collision& check, Vector3& pos);
	void ResetAABB();
};
