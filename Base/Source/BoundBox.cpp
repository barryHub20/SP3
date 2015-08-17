//#include "BoundBox.h"
//using std::cin;
//using std::cout;
//using std::endl;
//
///* constructor destructor */
//BoundBox::BoundBox()
//{
//}
//
//BoundBox::~BoundBox()
//{
//}
//
///* Core */
///* core */
//void BoundBox::Set(const Vector3& objectPos, Vector3 scale)
//{
//	/* public access variables */
//	scale = scale;
//
//	/* internal variables */
//	position = objectPos;
//	previousPos = position;
//	start = position - (this->scale * 0.5f);
//	end = position + (this->scale * 0.5f);
//	previousStart = previousPos - (this->scale * 0.5f);
//	previousEnd = previousPos + (this->scale * 0.5f);
//
//	/* flags */
//	collideArea.Reset();
//	total = 0;
//}
//
//void BoundBox::Update(const Vector3& objectPos)
//{
//	position = objectPos;
//	/* update start and end */
//	start = position - (this->scale * 0.5f);
//	end = position + (this->scale * 0.5f);
//}
//
//void BoundBox::UpdateCollide(CollideBox* check, Vector3& pos, Movement_3d& collidePart)
//{
//	/* check collide */
//	/* check collision first */
//
//	if(collidePart.collideSide == Movement_3d::start_X)
//	{
//		pos.x = previousPos.x;
//	}
//	else if(collidePart.collideSide == Movement_3d::end_X)
//	{
//		pos.x = previousPos.x;
//	}
//
//	//y
//	else if(collidePart.collideSide == Movement_3d::start_Y)
//	{
//		pos.y = previousPos.y;
//	}
//	else if(collidePart.collideSide == Movement_3d::end_Y)
//	{
//		pos.y = previousPos.y;
//	}
//
//	//z
//	else if(collidePart.collideSide == Movement_3d::start_Z)
//	{
//		pos.z = previousPos.z;
//	}
//	else if(collidePart.collideSide == Movement_3d::end_Z)
//	{
//		pos.z = previousPos.z;
//	}
//}
//
///** check if bound box <start to end> is in another BoundBox_3D's zone **/
//bool inZone(float& start, float& end, float& checkStart, float& checkEnd)
//{
//	return (end > checkStart && start < checkEnd);
//}
//
//void getCollide(float& start, float& end, float& checkStart, float& checkEnd, Movement_3d::COLLIDE startDir, Movement_3d::COLLIDE endDir, Movement_3d::COLLIDE& collideSide)
//{
//	if(start > checkEnd)
//	{
//		collideSide = startDir;
//	}
//	else if(end < checkStart)
//	{
//		collideSide = endDir;
//	}
//}
//
//void BoundBox::CheckCollide(CollideBox* check, Vector3& position)
//{
//	BoundBox* checkBox = dynamic_cast<BoundBox*>(check);
//
//	++total;
//	/*********************** check direction X, Y and Z ***********************/
//	inZoneY = inZone(start.y, end.y, checkBox->start.y, checkBox->end.y);
//	inZoneX = inZone(start.x, end.x, checkBox->start.x, checkBox->end.x);
//	inZoneZ = inZone(start.z, end.z, checkBox->start.z, checkBox->end.z);
//
//	/** if collide **/
//	if(inZoneY && inZoneX && inZoneZ)
//	{
//		inZoneY = inZone(previousStart.y, previousEnd.y, checkBox->start.y, checkBox->end.y);
//		inZoneX = inZone(previousStart.x, previousEnd.x, checkBox->start.x, checkBox->end.x);
//		inZoneZ = inZone(previousStart.z, previousEnd.z, checkBox->start.z, checkBox->end.z);
//
//
//		///** Y dir **/
//		if(inZoneX && inZoneZ && !inZoneY)
//		{
//			//start/end
//			getCollide(previousStart.y, previousEnd.y, checkBox->start.y, checkBox->end.y, Movement_3d::start_Y, Movement_3d::end_Y, collideArea.collideSide);
//		}
//
//		/** X dir **/
//		else if(inZoneY && inZoneZ && !inZoneX)
//		{
//			//start/end
//			getCollide(previousStart.x, previousEnd.x, checkBox->start.x, checkBox->end.x, Movement_3d::start_X, Movement_3d::end_X, collideArea.collideSide);
//		}
//
//		/** Z dir **/
//		else if(inZoneY && inZoneX && !inZoneZ)
//		{
//			//start/end
//			getCollide(previousStart.z, previousEnd.z, checkBox->start.z, checkBox->end.z, Movement_3d::start_Z, Movement_3d::end_Z, collideArea.collideSide);
//		}
//
//		//position = previousPos;
//		UpdateCollide(checkBox, position, collideArea);
//		start = position - (this->scale * 0.5f);
//		end = position + (this->scale * 0.5f);
//	}
//
//	/** update previous **/
//	if(total == 3)
//	{
//		previousPos = position;
//		previousStart = start;
//		previousEnd = end;
//	}
//}
//
//void BoundBox::Reset()
//{
//	total = 0;
//	/*previousPos = position;
//	previousStart = start;
//	previousEnd = end;*/
//}
//
//
//Vector3 BoundBox::getStart(){return start;}
//Vector3 BoundBox::getEnd(){return end;}
//Vector3 BoundBox::getRadius(){return scale * 0.5;}
//Vector3 BoundBox::getScale(){return scale;}
//Movement_3d* const BoundBox::getMovement_3d(){return &collideArea;}