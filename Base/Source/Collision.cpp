#include "Collision.h"

/* AABB Collision */
Vector3 currentStart;	//current collide bound
Vector3 currentEnd;
Vector3 previousStart;	//current collide bound @ previous frame 
Vector3 previousEnd;
Vector3 checkStart;	//to be checked with collide bound
Vector3 checkEnd;
bool inZoneX, inZoneY, inZoneZ;

Collision::Collision()
{
}

/* Init */
void Collision::Set(Vector3 pos, Vector3 scale, TYPE type)
{
	position = pos;
	previousPos = pos;
	this->scale = scale;
	this->type = type;
}

/* Update */
void Collision::Start(const Vector3& objectPos)
{
	if(type == BOX)
	{
		ResetAABB();
		position = objectPos;
	
		/* update currentStart and currentEnd (For AABB) */
		currentStart = position - (this->scale * 0.5f);
		currentEnd = position + (this->scale * 0.5f);
	}
}

bool Collision::CheckCollision(Collision& check)
{
	//see current type is what type of collide bound
	switch(type)
	{
	case BOX:	//current type: box --- check type: ??
		return CheckBoxDetection(check);
		break;
	case SPHERE:	//current type: sphere --- check type: ??
		return CheckSphereDetection(check);
		break;
	case SLANTED_BOX:

		break;
	}
	return false;
}

Collision::~Collision()
{

}

bool Collision::CheckSphereDetection(Collision& check)
{
	//check other bound is of what type 
	switch (check.type)
	{
	case BOX:	//current type: sphere ---- Check type: box
		//return AABB_CheckCollision(check);
		break;
	case SPHERE:	//current type: sphere ---- Check type: sphere
		//return AABB_CheckCollision(check);
		break;
	}

	return false;
}

bool Collision::CheckBoxDetection(Collision& check)
{
	//check other bound is of what type 
	switch (check.type)
	{
	case BOX:	//current type: box ---- Check type: box
		return AABB_CheckCollision(check);
		break;
	case SPHERE:	//current type: box ---- Check type: sphere
		//return AABB_CheckCollision(check);
		break;
	}

	return false;
}

void Collision::Reset()
{
	if(type == BOX)
	{
		ResetAABB();
	}
}

/* Sphere */
bool Sphere_To_Box_CheckCollision(Collision& check)
{
	/* check if got collision */
	//current type == sphere
	//check type == box

	/* Set up check object currentStart and currentEnd */
	checkStart = check.position - check.scale * 0.5f;
	checkEnd = check.position + check.scale * 0.5f;

	/* 4 points of a circle */
	Vector3 lowerLeft, upperRight;

	return false;
}

/* AABB */
bool Collision::AABB_CheckCollision(Collision& check)
{
	/* Set up check object currentStart and currentEnd */
	checkStart = check.position - check.scale * 0.5f;
	checkEnd = check.position + check.scale * 0.5f;

	/*********************** check collision ***********************/
	inZoneY = inZone(currentStart.y, currentEnd.y, checkStart.y, checkEnd.y);
	inZoneX = inZone(currentStart.x, currentEnd.x, checkStart.x, checkEnd.x);
	inZoneZ = inZone(currentStart.z, currentEnd.z, checkStart.z, checkEnd.z);

	/*********************** If Collide ***********************/
	if(inZoneX && inZoneY && inZoneZ)
	{
		inZoneY = inZone(previousStart.y, previousEnd.y, checkStart.y, checkEnd.y);
		inZoneX = inZone(previousStart.x, previousEnd.x, checkStart.x, checkEnd.x);
		inZoneZ = inZone(previousStart.z, previousEnd.z, checkStart.z, checkEnd.z);


		///** Y dir **/
		if(inZoneX && inZoneZ && !inZoneY)
		{
			getAABBCollide(previousStart.y, previousEnd.y, checkStart.y, checkEnd.y, Movement_3d::start_Y, Movement_3d::end_Y, collideArea.collideSide);
		}

		/** X dir **/
		else if(inZoneY && inZoneZ && !inZoneX)
		{
			getAABBCollide(previousStart.x, previousEnd.x, checkStart.x, checkEnd.x, Movement_3d::start_X, Movement_3d::end_X, collideArea.collideSide);
		}

		/** Z dir **/
		else if(inZoneY && inZoneX && !inZoneZ)
		{
			getAABBCollide(previousStart.z, previousEnd.z, checkStart.z, checkEnd.z, Movement_3d::start_Z, Movement_3d::end_Z, collideArea.collideSide);
		}

		UpdateAABB(check, position);	//update position of boundbox
		currentStart = position - (this->scale * 0.5f);	//currentStart and currentEnd too
		currentEnd = position + (this->scale * 0.5f);

		return true;
	}

	return false;
}

void Collision::UpdateAABB(Collision& check, Vector3& pos)
{
	float offset = 0.8f;	//offset to translate not directly at the pos
	Vector3 halfScale = scale * 0.5f;
	/* check collide */
	/* check collision first */

	if(collideArea.collideSide == Movement_3d::start_X)
	{
		//pos.x = previousPos.x;
		pos.x = checkEnd.x + halfScale.x + offset;
	}
	else if(collideArea.collideSide == Movement_3d::end_X)
	{
		pos.x = checkStart.x - halfScale.x - offset;
	}

	//y
	else if(collideArea.collideSide == Movement_3d::start_Y)
	{
		pos.y = checkEnd.y + halfScale.y + offset;
	}
	else if(collideArea.collideSide == Movement_3d::end_Y)
	{
		pos.y = checkStart.y - halfScale.y - offset;
	}

	//z
	else if(collideArea.collideSide == Movement_3d::start_Z)
	{
		pos.z = checkEnd.z + halfScale.z + offset;
	}
	else if(collideArea.collideSide == Movement_3d::end_Z)
	{
		pos.z = checkStart.z - halfScale.z - offset;
	}
}

bool Collision::inZone(float& currentStart, float& currentEnd, float& checkStart, float& checkEnd)
{
	return (currentEnd > checkStart && currentStart < checkEnd);
}

void Collision::getAABBCollide(float& currentStart, float& currentEnd, float& checkStart, float& checkEnd, Movement_3d::COLLIDE startDir, Movement_3d::COLLIDE endDir, Movement_3d::COLLIDE& collideSide)
{
	if(currentStart >= checkEnd)
	{
		collideSide = startDir;
	}
	else if(currentEnd <= checkStart)
	{
		collideSide = endDir;
	}
}
void Collision::ResetAABB()
{
	previousPos = position;
	previousStart = position - scale * 0.5f;
	previousEnd = position + scale * 0.5f;
}
