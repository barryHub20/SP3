#include "BoundBox_2D.h"
using std::cin;
using std::cout;
using std::endl;

/******************************* Utilities ********************************/
/** check if bound box <start to end> is in another BoundBox_3D's zone **/
bool BoundBox_2D::inZone(float& start, float& end, float& checkStart, float& checkEnd)
{
	return (end > checkStart && start < checkEnd);
}

/* constructor destructor */
BoundBox_2D::BoundBox_2D()
{
}

BoundBox_2D::~BoundBox_2D()
{
}

/* core */
void BoundBox_2D::Set(const Vector3& objectPos, Vector3 scale)
{
	CollideBox::Set(objectPos, scale);

	/* flags */
	inZoneY = inZoneX = false;
}

void BoundBox_2D::Update(const Vector3& objectPos)
{
	position = objectPos;
	/* update start and end */
	start = position - (this->scale * 0.5f);
	end = position + (this->scale * 0.5f);
}

void getCollide_2D(float& start, float& end, float& checkStart, float& checkEnd, Movement_3d::COLLIDE startDir, Movement_3d::COLLIDE endDir, Movement_3d::COLLIDE& collideSide)
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

void BoundBox_2D::UpdateCollide(BoundBox_2D* check, Vector3& pos, Movement_3d& collidePart)
{

	/* check collide */
	/* check collision first */

	if(collidePart.collideSide == Movement_3d::start_X)
	{
		pos.x = previousPos.x;
	}
	else if(collidePart.collideSide == Movement_3d::end_X)
	{
		pos.x = previousPos.x;
	}

	//y
	else if(collidePart.collideSide == Movement_3d::start_Y)
	{
		pos.y = previousPos.y;
	}
	else if(collidePart.collideSide == Movement_3d::end_Y)
	{
		pos.y = previousPos.y;
	}
}


void BoundBox_2D::CheckCollide(BoundBox_2D* checkBox, Vector3& position)
{
	++total;
	/*********************** check direction X, Y and Z ***********************/
	inZoneY = inZone(start.y, end.y, checkBox->start.y, checkBox->end.y);
	inZoneX = inZone(start.x, end.x, checkBox->start.x, checkBox->end.x);
	
	/** if collide **/
	if(inZoneY && inZoneX)
	{
		inZoneY = inZone(previousStart.y, previousEnd.y, checkBox->start.y, checkBox->end.y);
		inZoneX = inZone(previousStart.x, previousEnd.x, checkBox->start.x, checkBox->end.x);

		///** Y dir **/
		if(inZoneX && !inZoneY)
		{
			//start/end
			getCollide_2D(previousStart.y, previousEnd.y, checkBox->start.y, checkBox->end.y, Movement_3d::start_Y, Movement_3d::end_Y, collideArea.collideSide);
		}

		/** X dir **/
		else if(inZoneY && !inZoneX)
		{
			//start/end
			getCollide_2D(previousStart.x, previousEnd.x, checkBox->start.x, checkBox->end.x, Movement_3d::start_X, Movement_3d::end_X, collideArea.collideSide);
		}

		//position = previousPos;
		UpdateCollide(checkBox, position, collideArea);
		start = position - (this->scale * 0.5f);
		end = position + (this->scale * 0.5f);
	}

	/** update previous **/
	if(total == 3)
	{
		previousPos = position;
		previousStart = start;
		previousEnd = end;
	}
}

void BoundBox_2D::Reset()
{
	total = 0;
	/*previousPos = position;
	previousStart = start;
	previousEnd = end;*/
}