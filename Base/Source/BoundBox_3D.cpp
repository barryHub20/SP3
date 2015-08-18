#include "BoundBox_3D.h"
using std::cin;
using std::cout;
using std::endl;

/******************************* Utilities ********************************/
/** check if bound box <start to end> is in another BoundBox_3D's zone **/
bool BoundBox_3D::inZone(float& start, float& end, float& checkStart, float& checkEnd)
{
	return (end > checkStart && start < checkEnd);
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
	start = position - (this->scale * 0.5f);
	end = position + (this->scale * 0.5f);
}

void getCollide(float& start, float& end, float& checkStart, float& checkEnd, Movement_3d::COLLIDE startDir, Movement_3d::COLLIDE endDir, Movement_3d::COLLIDE& collideSide)
{
	if(start > checkEnd)
	{
		collideSide = startDir;
	}
	else if(end < checkStart)
	{
		collideSide = endDir;
	}
}

void BoundBox_3D::UpdateCollide(BoundBox_3D* check, Vector3& pos)
{
	//float offset = 0.1f;	//offset to translate not directly at the pos
	//Vector3 halfScale = scale * 0.5f;
	///* check collide */
	///* check collision first */
	//Vector3 posx = pos;

	//if(collideArea.collideSide == Movement_3d::start_X)
	//{
	//	//pos.x = previousPos.x;
	//	posx.x = check->end.x + halfScale.x + offset;
	//}
	//else if(collideArea.collideSide == Movement_3d::end_X)
	//{
	//	posx.x = check->start.x - halfScale.x - offset;
	//}

	////y
	//else if(collideArea.collideSide == Movement_3d::start_Y)
	//{
	//	posx.y = check->end.y + halfScale.y + offset;
	//}
	//else if(collideArea.collideSide == Movement_3d::end_Y)
	//{
	//	posx.y = check->start.y - halfScale.y - offset;
	//}

	////z
	//else if(collideArea.collideSide == Movement_3d::start_Z)
	//{
	//	posx.z = check->end.z + halfScale.z + offset;
	//}
	//else if(collideArea.collideSide == Movement_3d::end_Z)
	//{
	//	posx.z = check->start.z - halfScale.z - offset;
	//}

	/* check collide */
	/* check collision first */
	if(collideArea.collideSide == Movement_3d::start_X)
	{
		pos.x = previousPos.x;
	}
	else if(collideArea.collideSide == Movement_3d::end_X)
	{
		pos.x = previousPos.x;
	}

	//y
	else if(collideArea.collideSide == Movement_3d::start_Y)
	{
		pos.y = previousPos.y;
	}
	else if(collideArea.collideSide == Movement_3d::end_Y)
	{
		pos.y = previousPos.y;
	}

	//z
	else if(collideArea.collideSide == Movement_3d::start_Z)
	{
		pos.z = previousPos.z;
	}
	else if(collideArea.collideSide == Movement_3d::end_Z)
	{
		pos.z = previousPos.z;
	}
}

Vector3 current_pos;	
void BoundBox_3D::Start(Vector3& objectPos)
{
	Update(objectPos);
}

bool BoundBox_3D::Collide(BoundBox_3D* checkBox)
{
	/*********************** check collision ***********************/
	inZoneY = inZone(start.y, end.y, checkBox->start.y, checkBox->end.y);
	inZoneX = inZone(start.x, end.x, checkBox->start.x, checkBox->end.x);
	inZoneZ = inZone(start.z, end.z, checkBox->start.z, checkBox->end.z);
	
	/** if collide **/
	return (inZoneY && inZoneX && inZoneZ);
}

Vector3 BoundBox_3D::Response(BoundBox_3D* checkBox)
{
	inZoneY = inZone(previousStart.y, previousEnd.y, checkBox->start.y, checkBox->end.y);
	inZoneX = inZone(previousStart.x, previousEnd.x, checkBox->start.x, checkBox->end.x);
	inZoneZ = inZone(previousStart.z, previousEnd.z, checkBox->start.z, checkBox->end.z);


	///** Y dir **/
	if(inZoneX && inZoneZ && !inZoneY)
	{
		//start/end
		getCollide(previousStart.y, previousEnd.y, checkBox->start.y, checkBox->end.y, Movement_3d::start_Y, Movement_3d::end_Y, collideArea.collideSide);
	}

	/** X dir **/
	else if(inZoneY && inZoneZ && !inZoneX)
	{
		//start/end
		getCollide(previousStart.x, previousEnd.x, checkBox->start.x, checkBox->end.x, Movement_3d::start_X, Movement_3d::end_X, collideArea.collideSide);
	}

	/** Z dir **/
	else if(inZoneY && inZoneX && !inZoneZ)
	{
		//start/end
		getCollide(previousStart.z, previousEnd.z, checkBox->start.z, checkBox->end.z, Movement_3d::start_Z, Movement_3d::end_Z, collideArea.collideSide);
	}


	UpdateCollide(checkBox, position);	//update position of boundbox
	start = position - (this->scale * 0.5f);	//start and end too
	end = position + (this->scale * 0.5f);

	return position;
}

void BoundBox_3D::Reset()
{
	previousPos = position;
	previousStart = start;
	previousEnd = end;
}
