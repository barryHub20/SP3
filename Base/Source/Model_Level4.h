#ifndef Model_Level4_H
#define Model_Level4_H
#include "Model_Level.h"

class Model_Level4 : public Model_Level
{
private:
	static int triggerObjectSize;
	GameObject *doors[2];
	Item *keys[2];
	Item *puzzleNote;
	bool firstKey;
	bool secondKey;
	GameObject *staircases[2];

	TriggerArea keyActiveArea[2];

	TriggerObject *spikeTraps[3];

	Ogre* ogres[3];

	float invulerabilityFrame;
	float damageTimer;
	float ogreTimer;
public:
	/*********** constructor/destructor ***************/
	Model_Level4();
	~Model_Level4();

	/*********** core functions ***************/
	//Initialize
	virtual void Init();
	void InitTrigger();
	void InitUI();
	void InitObject();
	void InitPuzzles();
	void spawnItems();

	//Update
	virtual void Update(double dt, bool* myKeys, Vector3 mousePos, StateManager::STATES currentState);
	void UpdateGame(double dt, bool* myKeys);
	void UpdateEnemy(double dt);
	void UpdateTraps(double dt, bool* myKeys);

	virtual void ClearLevel();
	virtual void Exit();
};

#endif