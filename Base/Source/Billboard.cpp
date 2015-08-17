#include "Billboard.h"
/* constructor/destructor */
Billboard::Billboard()
{
}

Billboard::~Billboard()
{
}

/* core */
void Billboard::Set(string name, Mesh* mesh, Vector3 scale, Vector3 position, Object* parent, bool light)
{
	Object::Set(name, mesh, scale, position, parent, light, false);
}

Vector3 camDir;
void Billboard::Update(Camera3& cam)
{   
	camDir = (cam.position - position).Normalized();
	
	/* get angle (rotate along Y angle) */
	currentAngleY =  Math::RadianToDegree(atan2(camDir.x, camDir.z));
}

/* getter/setter */

