#include "Camera_2D.h"
using namespace std;

Camera2D::Camera2D()
{
}

Camera2D::~Camera2D()
{
}

void Camera2D::Init(const Vector3& pos, const Vector3& target, const Vector3& up, float DeadZone_Width, float DeadZone_Height, float viewWidth, float viewHeight)
{
	this->position = pos;
	this->target = target;

	this->target = this->target - target;

	DeadZone.x = DeadZone_Width * 0.5f;	//deadzone is half of the scale of the "box" area
	DeadZone.y = DeadZone_Height * 0.5f;
	DeadZone.z = 0;

	this->viewWidth = viewWidth * 0.5f;
	this->viewHeight = viewHeight * 0.5f;
}

float startX, endX, startY, endY;
void Camera2D::SetBound(float xMapScale, float yMapScale)
{
	Vector3 middlePos(xMapScale * 0.5f, yMapScale * 0.5f);

	/* position is offset: in case not start at 0,0 */
	middlePos += position * 0.5f;

	if(position.y != 0)
		yMapScale -= position.y;
	if(position.x != 0)
		xMapScale -= position.x;

	/** set boundary **/
	boundStart.x = middlePos.x - xMapScale * 0.5f;
	boundEnd.x = middlePos.x + xMapScale * 0.5f;

	boundStart.y = middlePos.y - yMapScale * 0.5f;
	boundEnd.y = middlePos.y + yMapScale * 0.5f;

	if(position.x <= boundStart.x)
	{
		position.x = boundStart.x;
	}

	else if(position.x + (viewWidth * 2.f) >= boundEnd.x)
	{
		position.x = boundEnd.x - (viewWidth * 2.f);
	}


	/* check if Y go out of bounds */

	if(position.y <= boundStart.y)
	{
		position.y = boundStart.y;
	}


	else if(position.y + (viewHeight * 2.f) >= boundEnd.y)
	{
		position.y = boundEnd.y - (viewHeight * 2.f);
	}
}

void Camera2D::Update(double dt, const Vector3& currentPos, const Vector3& scale)
{
	startX = position.x + (viewWidth - DeadZone.x);
	endX = (position.x + viewWidth * 2.f) - (viewWidth - DeadZone.x);
	startY = position.y + (viewHeight - DeadZone.y);
	endY = (position.y + viewHeight * 2.f) - (viewHeight - DeadZone.y);
	
	if( currentPos.x - scale.x * 0.5f < startX )	
	{
		if(position.x > boundStart.x)
			position.x -= startX - (currentPos.x - scale.x * 0.5f);
		else
			position.x = boundStart.x;
	}
	else if( currentPos.x + scale.x * 0.5f > endX )	
	{
		if(position.x + (viewWidth * 2.f) < boundEnd.x)
			position.x += (currentPos.x + scale.x * 0.5f) - endX;
		else
			position.x = boundEnd.x - (viewWidth * 2.f);
	}

	/* check if Y go out of bounds */
	if( currentPos.y - scale.y * 0.5f < startY )	
	{
		if(position.y > boundStart.y)
			position.y -= startY - (currentPos.y - scale.y * 0.5f); 
		else
			position.y = boundStart.y;
	}
	else if( currentPos.y + scale.y * 0.5f > endY )	
	{
		if(position.y + (viewHeight * 2.f) < boundEnd.y)
			position.y += (currentPos.y + scale.y * 0.5f) - endY;
		else
			position.y = boundEnd.y - (viewHeight * 2.f);
	}
}

void Camera2D::Reset()
{
}
