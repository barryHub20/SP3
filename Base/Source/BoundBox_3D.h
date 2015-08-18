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
	void Reset();
	virtual void Update(const Vector3& objectPos);	//if object is static, do not need to update

	virtual bool Collide(BoundBox_3D* checkBox);	//detect if got collide
	virtual Vector3 Response(BoundBox_3D* checkBox);	//response
	void Start(Vector3& objectPos);

	/* Internal functions */
	bool inZone(float& start, float& end, float& checkStart, float& checkEnd);
	void UpdateCollide(BoundBox_3D* check, Vector3& pos);
};

#endif