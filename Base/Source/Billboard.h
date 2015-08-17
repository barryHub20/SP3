#ifndef BILLBOARD_H
#define BILLBOARD_H
#include "Object.h"
#include "Camera3.h"

class Billboard : public Object
{
public:
	/* constructor/destructor */
	Billboard();
	~Billboard();

	/* core */
	void Set(string name, Mesh* mesh, Vector3 scale, Vector3 position, Object* parent, bool light);
	void Update(Camera3& cam);

	/* getter/setter */

	
	float currentAngleY;
};

#endif