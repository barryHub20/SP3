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
}

Collision::~Collision()
{

}

bool Collision::CheckSphereDetection(Collision& check)
{
}

bool Collision::CheckBoxDetection(Collision& check)
{
}