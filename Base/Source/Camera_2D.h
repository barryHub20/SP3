#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include "Camera.h"

class Camera2D : public Camera
{
	Vector3 DeadZone;	//boundbox area
	float viewWidth;	//width of camera view in real world
	float viewHeight;	//height of camera view in real world
	Vector3 boundStart;	//boundary camera can offset
	Vector3 boundEnd;
public:	
	Camera2D();
	~Camera2D();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up, float DeadZone_Width, float DeadZone_Height, float viewWidth, float viewHeight, float xMapScale, float yMapScale);
	void SetBound(const Vector3& currentPos);
	void Update(double dt, const Vector3& currentPos, const Vector3& scale);
	virtual void Reset();
};

#endif