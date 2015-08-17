#ifndef BULLET_H
#define BULLET_H
#include "Object.h"
#include "Vector2.h"
#include "Map.h"

class Bullet : public Object
{
public:
	/* static variables */
	const static float MAX_SPEED;
	const static double MAX_LIFESPAN;

	/* constructor/destructor */
	Bullet();
	~Bullet();

	/* update */
	void Set(Mesh* mesh, float posX, float posY, float speed, bool light);
	void Fire(const Vector2& pos, const Vector2& dir, bool friendly);
	void Update(const double& dt, Map* map, Vector2& offset);
	void Deactivate();

	/* var */
	int checkPosition_X;	//tile map
	int checkPosition_Y;	//tile map
	Vector2 pos;
	Vector2 vel;
	double timer;
	bool friendly;	//is it friendly to player?
};

#endif