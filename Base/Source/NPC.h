#ifndef NPC_H
#define NPC_H
#include "Character.h"
#include "Strategy_Patrol.h"
#include "Strategy_Attack.h"

class NPC : public Character
{
protected:
	Vector2 offset;	//offset pos
	Vector2 originalPos;	//for offset when scrolling
	bool offsetPos;
	float dist_From_Enemy;	//dist to trigger attack

	/* stuff */
	Strategy* strategy_pack[Strategy::TOTAL_STRATEGY];
	Strategy::STRATEGY_TYPE currentStrategy;	//current strategy type

	double fireRate;
	double fireTimer;

public:
	bool shotLeft;	//is npc shot on left side?
	bool shotRight;	//is npc shot on right side?
	bool fireLeft;	//npc fired(attack)?
	bool fireRight;	//npc fired(attack)?

/* constructor/destructor */
	NPC();
	~NPC();

	/* core */
	//init
	virtual void Init(SpriteAnimation* anim_mesh, float xPos, float yPos, float xScale, float yScale, Map* map);
	void InitStrategy(Character* enemy);
	void setStrategy(Character* enemy);

	//update
	//runs strategy: to be called by child class so whenever strategy to be used, it will be called
	void RunStrategy(double dt, Vector2& offset, Map* map, Character* enemy);
	void Update(double dt, Vector2& offset, Map* map, Character* enemy);

	/* movement */
	virtual void Jump(double dt);
	virtual void MoveLeftRight(bool mode, double dt);
};

#endif