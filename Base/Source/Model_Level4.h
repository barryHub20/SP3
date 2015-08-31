#ifndef Model_Level4_H
#define Model_Level4_H
#include "Model_Level.h"

class Model_Level4 : public Model_Level
{
private:
	static int triggerObjectSize;
	GameObject *doors[2];
	Item *keys[2];
	Item *puzzleNotes[2];
	bool pickedUpKeys[2];
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
	virtual void Update(double dt, bool* myKeys, Vector3 mousePos);
	void UpdateGame(double dt, bool* myKeys);
	void UpdateEnemy(double dt);
	void UpdateTraps(double dt, bool* myKeys);


	virtual void Exit();
};

#endif