#include "Collision.h"

/* AABB Collision */
Vector3 currentStart;	//current collide bound
Vector3 currentEnd;
Vector3 previousStart;	//current collide bound @ previous frame 
Vector3 previousEnd;
Vector3 checkStart;	//to be checked with collide bound
Vector3 checkEnd;
bool inZoneX, inZoneY, inZoneZ;
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
		ResetAABB();
		position = objectPos;
	
		/* update currentStart and currentEnd (For AABB) */
		currentStart = position - (this->scale * 0.5f);
		currentEnd = position + (this->scale * 0.5f);
	}
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
		cout << "sdfdsf" << endl;
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
	/* Set up check object currentStart and currentEnd */
	checkStart = check->position - check->scale * 0.5f;
	checkEnd = check->position + check->scale * 0.5f;

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
			getAABBCollide(previousStart.y, previousEnd.y, checkStart.y, checkEnd.y, Movement_3d::start_Y, Movement_3d::end_Y, current->collideArea.collideSide);
		}

		/** X dir **/
		else if(inZoneY && inZoneZ && !inZoneX)
		{
			getAABBCollide(previousStart.x, previousEnd.x, checkStart.x, checkEnd.x, Movement_3d::start_X, Movement_3d::end_X, current->collideArea.collideSide);
		}

		/** Z dir **/
		else if(inZoneY && inZoneX && !inZoneZ)
		{
			getAABBCollide(previousStart.z, previousEnd.z, checkStart.z, checkEnd.z, Movement_3d::start_Z, Movement_3d::end_Z, current->collideArea.collideSide);
		}

		UpdateAABB(current, check);	//update position of boundbox
		currentStart = current->position - (current->scale * 0.5f);	//currentStart and currentEnd too
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


void Collision::UpdateAABB(Collision* current, Collision* check)
{
	Vector3 halfScale = current->scale * 0.5f;
	/* check collide */
	/* check collision first */

	if(current->collideArea.collideSide == Movement_3d::start_X)
	{
		//pos.x = previousPos.x;
		current->position.x = checkEnd.x + halfScale.x + offset;
	}
	else if(current->collideArea.collideSide == Movement_3d::end_X)
	{
		current->position.x = checkStart.x - halfScale.x - offset;
	}

	//y
	else if(current->collideArea.collideSide == Movement_3d::start_Y)
	{
		current->position.y = checkEnd.y + halfScale.y + offset;
	}
	else if(current->collideArea.collideSide == Movement_3d::end_Y)
	{
		current->position.y = checkStart.y - halfScale.y - offset;
	}

	//z
	else if(current->collideArea.collideSide == Movement_3d::start_Z)
	{
		current->position.z = checkEnd.z + halfScale.z + offset;
	}
	else if(current->collideArea.collideSide == Movement_3d::end_Z)
	{
		current->position.z = checkStart.z - halfScale.z - offset;
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
