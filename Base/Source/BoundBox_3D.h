#ifndef BBOX_3D_H
#define BBOX_3D_H
#include "BoundBox.h"

/** 3D bound Box **/
class BoundBox_3D : public CollideBox
{
private:
	/* flag */
	bool inZoneX, inZoneY, inZoneZ;

public:

	/* constructor destructor */
	BoundBox_3D();
	~BoundBox_3D();

	/* core */
	virtual void Set(const Vector3& objectPos, Vector3 scale);
	virtual void Reset();
	virtual void Update(const Vector3& objectPos);	//if object is static, do not need to update

	/* utilities */
	bool inZone(float& start, float& end, float& checkStart, float& checkEnd);
	virtual bool CheckCollide(BoundBox_3D* check, Vector3& position);
	virtual void UpdateCollide(BoundBox_3D* check, Vector3& pos);
};

#endif