#include "BoundBox_3D.h"
using std::cin;
using std::cout;
using std::endl;
Vector3 _previousStart;
Vector3 _previousEnd;
Vector3 _currentStart;
Vector3 _currentEnd;
Vector3 _checkStart;
Vector3 _checkEnd;

/******************************* Utilities ********************************/
/** check if bound box <start to end> is in another BoundBox_3D's zone **/
bool BoundBox_3D::inZone(float& start, float& end, float& _checkStart, float& _checkEnd)
{
	return (end >= _checkStart && start <= _checkEnd);
}


/* constructor destructor */
BoundBox_3D::BoundBox_3D()
{
}

BoundBox_3D::~BoundBox_3D()
{
}

/* core */
void BoundBox_3D::Set(const Vector3& objectPos, Vector3 scale)
{
	CollideBox::Set(objectPos, scale);

	/* flags */
	inZoneY = inZoneX = inZoneZ = false;
}

void BoundBox_3D::Update(const Vector3& objectPos)
{
	position = objectPos;
	/* update start and end */
	_currentStart = position - (this->scale * 0.5f);
	_currentEnd = position + (this->scale * 0.5f);
}

void getCollide(float& start, float& end, float& _checkStart, float& _checkEnd, Movement_3d::COLLIDE startDir, Movement_3d::COLLIDE endDir, Movement_3d::COLLIDE& collideSide)
{
	if(start > _checkEnd)
	{
		collideSide = startDir;
	}
	else if(end < _checkStart)
	{
		collideSide = endDir;
	}
}

void BoundBox_3D::UpdateCollide(BoundBox_3D* check, Vector3& pos)
{
	float offset = 0.1f;	//offset to translate not directly at the pos
	Vector3 halfScale = scale * 0.5f;
	/* check collide */
	/* check collision first */

	if(collideArea.collideSide == Movement_3d::start_X)
	{
		//pos.x = previousPos.x;
		pos.x = _checkEnd.x + halfScale.x + offset;
	}
	else if(collideArea.collideSide == Movement_3d::end_X)
	{
		pos.x = _checkStart.x - halfScale.x - offset;
	}

	//y
	else if(collideArea.collideSide == Movement_3d::start_Y)
	{
		pos.y = _checkEnd.y + halfScale.y + offset;
	}
	else if(collideArea.collideSide == Movement_3d::end_Y)
	{
		pos.y = _checkStart.y - halfScale.y - offset;
	}

	//z
	else if(collideArea.collideSide == Movement_3d::start_Z)
	{
		pos.z = _checkEnd.z + halfScale.z + offset;
	}
	else if(collideArea.collideSide == Movement_3d::end_Z)
	{
		pos.z = _checkStart.z - halfScale.z - offset;
	}

	///* check collide */
	///* check collision first */
	//if(collideArea.collideSide == Movement_3d::start_X)
	//{
	//	pos.x = previousPos.x;
	//}
	//else if(collideArea.collideSide == Movement_3d::end_X)
	//{
	//	pos.x = previousPos.x;
	//}

	////y
	//else if(collideArea.collideSide == Movement_3d::start_Y)
	//{
	//	pos.y = previousPos.y;
	//}
	//else if(collideArea.collideSide == Movement_3d::end_Y)
	//{
	//	pos.y = previousPos.y;
	//}

	////z
	//else if(collideArea.collideSide == Movement_3d::start_Z)
	//{
	//	pos.z = previousPos.z;
	//}
	//else if(collideArea.collideSide == Movement_3d::end_Z)
	//{
	//	pos.z = previousPos.z;
	//}
}

Vector3 current_pos;	
void BoundBox_3D::Start(Vector3& objectPos)
{
	position = objectPos;

	/* update start and end */
	_currentStart = position - (this->scale * 0.5f);
	_currentEnd = position + (this->scale * 0.5f);

	_previousStart = previousPos - (this->scale * 0.5f);
	_previousEnd = previousPos + (this->scale * 0.5f);
}

bool BoundBox_3D::Collide(BoundBox_3D* checkBox)
{
	/* chck box */
	_checkStart = checkBox->position - (checkBox->scale * 0.5f);
	_checkEnd = checkBox->position + (checkBox->scale * 0.5f);

	/*********************** check collision ***********************/
	inZoneY = inZone(_currentStart.y, _currentEnd.y, _checkStart.y, _checkEnd.y);
	inZoneX = inZone(_currentStart.x, _currentEnd.x, _checkStart.x, _checkEnd.x);
	inZoneZ = inZone(_currentStart.z, _currentEnd.z, _checkStart.z, _checkEnd.z);
	
	/** if collide **/
	return (inZoneY && inZoneX && inZoneZ);
}

Vector3 BoundBox_3D::Response(BoundBox_3D* checkBox)
{
	inZoneY = inZone(_previousStart.y, _previousEnd.y, _checkStart.y, _checkEnd.y);
	inZoneX = inZone(_previousStart.x, _previousEnd.x, _checkStart.x, _checkEnd.x);
	inZoneZ = inZone(_previousStart.z, _previousEnd.z, _checkStart.z, _checkEnd.z);


	///** Y dir **/
	if(inZoneX && inZoneZ && !inZoneY)
	{
		//start/end
		getCollide(_previousStart.y, _previousEnd.y, _checkStart.y, _checkEnd.y, Movement_3d::start_Y, Movement_3d::end_Y, collideArea.collideSide);
	}

	/** X dir **/
	else if(inZoneY && inZoneZ && !inZoneX)
	{
		//start/end
		getCollide(_previousStart.x, _previousEnd.x, _checkStart.x, _checkEnd.x, Movement_3d::start_X, Movement_3d::end_X, collideArea.collideSide);
	}

	/** Z dir **/
	else if(inZoneY && inZoneX && !inZoneZ)
	{
		//start/end
		getCollide(_previousStart.z, _previousEnd.z, _checkStart.z, _checkEnd.z, Movement_3d::start_Z, Movement_3d::end_Z, collideArea.collideSide);
	}


	UpdateCollide(checkBox, position);	//update position of boundbox
	_currentStart = position - (this->scale * 0.5f);	//start and end too
	_currentEnd = position + (this->scale * 0.5f);

	return position;
}

void BoundBox_3D::Reset()
{
	previousPos = position;
}
