#include "Collision.h"

/* AABB Collision */
Vector3 currentStart;	//current collide bound
Vector3 currentEnd;
Vector3 previousStart;	//current collide bound @ previous frame 
Vector3 previousEnd;
Vector3 checkStart;	//to be checked with collide bound
Vector3 checkEnd;
bool inZoneX, inZoneY, inZoneZ;
Vector3 tmp_pos;
Vector3 relativeDir;	//relative direction (for spherical collision)
float offset = 0.1f;

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
		position = objectPos;

		/* update start and end */
		currentStart = position - (this->scale * 0.5f);
		currentEnd = position + (this->scale * 0.5f);

		previousStart = previousPos - (this->scale * 0.5f);
		previousEnd = previousPos + (this->scale * 0.5f);
	}
}

bool Collision::QuickAABBDetection(Collision* checkMe)
{
	currentStart = position - scale * 0.5f;
	currentEnd = position + scale * 0.5f;

	checkStart = checkMe->position - checkMe->scale * 0.5f;
	checkEnd = checkMe->position + checkMe->scale * 0.5f;

	return Collision::inZone(currentStart.x, currentEnd.x, checkStart.x, checkEnd.x) &&
		Collision::inZone(currentStart.y, currentEnd.y, checkStart.y, checkEnd.y);
}

Collision* a;
Collision* b;
bool Collision::CheckCollision(Collision& current, Collision& check)
{
	/************ Standardise ************/
	//BOX/SLANTED_BOX must always be current, unless current and check are both SPHERE
	a = &current;
	b = &check;

	/************ Reject ************/
	if(a->type == SLANTED_BOX && b->type == SLANTED_BOX)
		return false;

	else if( (a->type == BOX && b->type == SLANTED_BOX) ||
		(a->type == SLANTED_BOX && b->type == BOX) )
		return false;

	/************ Swap ************/
	//current type: sphere --- check tye: box ===> Swap
	//current type: sphere  --- check tye: slanted_box ===> Swap

	if(a->type == BOX || a->type == SLANTED_BOX)
	{
		if(b->type == SPHERE)
		{
			a = &check;
			b = &current;
		}
	}

	/************ Check collide ************/
	//possible combinations:
	//1) current type: Sphere --- Check type: Sphere 
	//3) current type: Sphere --- Check type: Slanted Box
	//4) current type: Sphere --- Check type: Box
	//2) current type: Box --- Check type: Box

	//see current type is what type of collide bound
	//1)
	if(a->type == SPHERE && b->type == SPHERE)
	{
		return SphereToSphere(a, b);
	}
	else if(a->type == SPHERE && b->type == SLANTED_BOX)
	{
		return SphereToSlantedBox(a, b);
	}
	else if(a->type == SPHERE && b->type == BOX)
	{
		return SphereToBox(a, b);
	}
	else if(a->type == BOX && b->type == BOX)
	{
		return BoxToBox(a, b);
	}

	return false;
}

Collision::~Collision()
{

}

/* Sphere (current) to Sphere (check) */
bool Collision::SphereToSphere(Collision* current, Collision* check)
{
	/* take scaleX as the radius */
	float combinedRadiusX = current->scale.x + check->scale.x;
	float distSquared = (current->position - check->position).LengthSquared();

	/* IF COLLIDE */
	if((combinedRadiusX * combinedRadiusX) < distSquared)
	{
		/* Offset pos of current to just touching the check object */

		/* Get the relative direction */
		if(!a->position.IsEqual(b->position.x, b->position.y))
			relativeDir = (a->position - b->position).Normalized();

		float combined_radius = a->scale.x + b->scale.x + offset;

		/* Set pos to just touching */
		Vector3 newPos = check->position + (relativeDir * combined_radius);
		check->position = newPos;
		return true;
	}
	return false;
}

/* Sphere (current) to Slanted Box (check) */
bool Collision::SphereToSlantedBox(Collision* current, Collision* check)
{

	return false;
}

/* Sphere (current) to Box (check) */
bool Collision::SphereToBox(Collision* current, Collision* check)
{
	/* check if got collision */

	/* Set up check object currentStart and currentEnd */
	checkStart = check->position - check->scale * 0.5f;
	checkEnd = check->position + check->scale * 0.5f;

	return false;
}

/* Box (current) to Box (check) */
bool Collision::BoxToBox(Collision* current, Collision* check)
{
	inZoneY = inZoneX = false;

	/* chck box */
	checkStart = check->position - (check->scale * 0.5f);
	checkEnd = check->position + (check->scale * 0.5f);

	/*********************** check collision ***********************/
	inZoneY = inZone(currentStart.y, currentEnd.y, checkStart.y, checkEnd.y);
	inZoneX = inZone(currentStart.x, currentEnd.x, checkStart.x, checkEnd.x);
	
	if(inZoneY && inZoneX)
	{
		inZoneY = inZone(previousStart.y, previousEnd.y, checkStart.y, checkEnd.y);
		inZoneX = inZone(previousStart.x, previousEnd.x, checkStart.x, checkEnd.x);

		///** Y dir **/
		if(inZoneX && !inZoneY)
		{
			//start/end
			getAABBCollide(previousStart.y, previousEnd.y, checkStart.y, checkEnd.y, Movement_3d::start_Y, Movement_3d::end_Y, current->collideArea.collideSide);
		}

		/** X dir **/
		else if(inZoneY && !inZoneX)
		{
			//start/end
			getAABBCollide(previousStart.x, previousEnd.x, checkStart.x, checkEnd.x, Movement_3d::start_X, Movement_3d::end_X, current->collideArea.collideSide);
		}


		UpdateAABB(current, check, current->position);	//update position of boundbox
		currentStart = current->position - (current->scale * 0.5f);	//start and end too
		currentEnd = current->position + (current->scale * 0.5f);

		return true;
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


void Collision::UpdateAABB(Collision* current, Collision* check, Vector3& pos)
{
	Vector3 halfScale = current->scale * 0.5f;
	/* check collide */
	/* check collision first */

	if(current->collideArea.collideSide == Movement_3d::start_X)
	{
		//pos.x = previousPos.x;
		pos.x = checkEnd.x + halfScale.x + offset;
	}
	else if(current->collideArea.collideSide == Movement_3d::end_X)
	{
		pos.x = checkStart.x - halfScale.x - offset;
	}

	//y
	else if(current->collideArea.collideSide == Movement_3d::start_Y)
	{
		pos.y = checkEnd.y + halfScale.y + offset;
	}
	else if(current->collideArea.collideSide == Movement_3d::end_Y)
	{
		pos.y = checkStart.y - halfScale.y - offset;
	}

	//z
	else if(current->collideArea.collideSide == Movement_3d::start_Z)
	{
		pos.z = checkEnd.z + halfScale.z + offset;
	}
	else if(current->collideArea.collideSide == Movement_3d::end_Z)
	{
		pos.z = checkStart.z - halfScale.z - offset;
	}
}

bool Collision::inZone(float& currentStart, float& currentEnd, float& checkStart, float& checkEnd)
{
	return (currentEnd >= checkStart && currentStart <= checkEnd);
}

void Collision::getAABBCollide(float& currentStart, float& currentEnd, float& checkStart, float& checkEnd, Movement_3d::COLLIDE startDir, Movement_3d::COLLIDE endDir, Movement_3d::COLLIDE& collideSide)
{
	if(currentStart > checkEnd)
	{
		collideSide = startDir;
	}
	else if(currentEnd < checkStart)
	{
		collideSide = endDir;
	}
}
void Collision::ResetAABB()
{
	previousPos = position;
}
