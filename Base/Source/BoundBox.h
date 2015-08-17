#ifndef BOUNDBOX_H
#define BOUNDBOX_H
#include <ostream>
#include "Vector3.h"
#include <vector>
using std::vector;

/**
store bool for all 6 sides of a cuboid collision box
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

/** Parent of all collision boxes **/
class CollideBox
{
public:
	enum TYPE
	{
		AABB_3D,
		AABB_2D,

		/* future */
		OBB_3D,
		OBB_2D,

		TOTAL_TYPE,
	};

	Movement_3d collideArea;	//which part collide	
	int total;

	/* constructor destructor */
	CollideBox();
	virtual ~CollideBox();

	/* core */
	virtual void Set(const Vector3& objectPos, Vector3 scale);
	virtual void Reset() = 0;
	virtual void Update(const Vector3& objectPos) = 0;	//if object is static, do not need to update

	/* utilities */
	//virtual void CheckCollide(CollideBox* checkBox, Vector3& position) = 0;
	//virtual void UpdateCollide(CollideBox* checkBox, Vector3& pos) = 0;

	/* geter/setter */
	Vector3 getScale();
	Vector3 getPosition();

protected:

	/* flag variables */
	Vector3 scale;	//if 2D, z == 0
	Vector3 position;
	Vector3 start;
	Vector3 end;// PUSH TEST IF GT PUSH FUCK GIT

	/* internal variables */
	Vector3 previousPos;
	Vector3 previousStart;
	Vector3 previousEnd;
};

#endif

