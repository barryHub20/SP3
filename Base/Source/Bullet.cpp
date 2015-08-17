#include "Bullet.h"

/* static variables */
const float Bullet::MAX_SPEED = 300.f;
const double Bullet::MAX_LIFESPAN = 5.0;

/* constructor/destructor */
Bullet::Bullet()
{
	pos.Set(0, 0);
	vel.Set(MAX_SPEED, MAX_SPEED);
}

Bullet::~Bullet()
{
}

/* update */
void Bullet::Set(Mesh* mesh, float posX, float posY, float speed, bool light)
{
	pos.Set(posX, posY);
	Object::Set("Bullet", mesh, Vector3(7, 4, 1), Vector3(posX, posY, 3), NULL, false, false);
	checkPosition_X = checkPosition_Y = 0;
	active = false;
	timer = 0.0;
	this->friendly = false;
}

void Bullet::Fire(const Vector2& pos, const Vector2& dir, bool friendly)
{
	active = true;

	this->friendly = friendly;
	this->vel.x *= dir.x;
	this->vel.y *= dir.y;
	timer = 0.0;
	this->pos.Set(pos.x, pos.y);
}

void Bullet::Update(const double& dt, Map* map, Vector2& offset)
{
	//dt: delta time
	//map: the map
	//offset: the scrolling offset
	Vector2 currentPos;	//pos taken into account with offset

	if(active)
	{
		pos.x += vel.x * dt;
		Object::translate(pos.x, pos.y, 3.f);
		timer += dt;

		///* update tile pos */
		currentPos = pos + offset;

		checkPosition_X = (int)(currentPos.x / map->GetTileSize());
		checkPosition_Y = (int)(currentPos.y) / map->GetTileSize();


		///* out of range */
		if( (checkPosition_Y <= 0 || checkPosition_Y >= map->GetNumOfTiles_Height()) ||
			(checkPosition_X <= 0 || checkPosition_X >= map->GetNumOfTiles_Width()) )
		{
			Deactivate();
		}

		/* Collides */
		if(active)
		{
			if(map->theScreenMap[checkPosition_Y][checkPosition_X] > 0)
				Deactivate();

			///* timer runs out */
			//if(timer >= MAX_LIFESPAN)
			//{
			//	Deactivate();
			//}
		}
	}
}

void Bullet::Deactivate()
{
	active = false;
	this->friendly = false;
	timer = 0.0;
	this->pos.Set(0, 0);
	vel.Set(MAX_SPEED, MAX_SPEED);
}