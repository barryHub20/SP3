#ifndef HERO_H
#define HERO_H
#include "Character.h"

class Hero : public Character
{
public:
	bool shotLeft;	//is npc shot on left side?
	bool shotRight;	//is npc shot on right side?
	bool fire;
	double FireRate;
	double FireTimer;
	int health;
	const static int MAX_HEALTH = 5;

	/* constructor/destructor */
	Hero();
	~Hero();

	/* core */
	virtual void Init(SpriteAnimation* anim_mesh, float xPos, float yPos, float xScale, float yScale, Map* map);
	void Update(double dt, bool* myKeys, float m_window_width, float m_window_height, Vector2& middlePos, const float constrain_DistX, Vector2& offset, Map* map);

	/* movement */
	void Constrain(float m_window_width, float m_window_height, Vector2& middlePos, const float constrain_DistX, Vector2& offset, Map* map);
	virtual void Jump(double dt);
	virtual void MoveLeftRight(bool mode, double dt);
};

#endif