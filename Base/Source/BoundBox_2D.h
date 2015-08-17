#ifndef BBOX_2D_H
#define BBOX_2D_H
#include "BoundBox.h"

/** Should be Vector2 or Vector3?
Vector2 saves 5 floats and 1 bool
Vector3 allows depth (Z) to be modified
**/

/** 3D bound Box **/
class BoundBox_2D : public CollideBox
{
private:
	/* flag variables */
	Vector3 start;
	Vector3 end;

	/* internal variables */
	Vector3 previousPos;
	Vector3 previousStart;
	Vector3 previousEnd;

	/* flag */
	bool inZoneX, inZoneY;

public:

	/* constructor destructor */
	BoundBox_2D();
	~BoundBox_2D();

	/* core */
	virtual void Set(const Vector3& objectPos, Vector3 scale);
	virtual void Reset();
	virtual void Update(const Vector3& objectPos);	//if object is static, do not need to update

	/* utilities */
	bool inZone(float& start, float& end, float& checkStart, float& checkEnd);
	virtual void CheckCollide(BoundBox_2D* checkBox, Vector3& position);
	virtual void UpdateCollide(BoundBox_2D* checkBox, Vector3& pos, Movement_3d& collidePart);
};

#endif