#ifndef BOUNDBOX_H
#define BOUNDBOX_H
#include <ostream>
#include "Vector3.h"
#include "Collision.h"
#include <vector>
using std::vector;


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
	virtual void Set(const Vector3& objectPos, Vector3 scale);	//Set all parameters
	virtual void Update(const Vector3& objectPos) = 0;	//Update pos, start and end
	
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

