#include "Collision.h"

Collision::Collision()
{
}

bool Collision::CheckCollision(Collision& check)
{
	switch(type)
	{
	case BOX:
		return CheckBoxDetection(check);
		break;
	case SPHERE:
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
	return true;
}

bool Collision::CheckBoxDetection(Collision& check)
{
	return true;
}