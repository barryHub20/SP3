#ifndef MODEL_LEVEL2_H
#define MODEL_LEVEL2_H
#include "Model_Level.h"

class Model_Level2 : public Model_Level
{
public:
/*********** constructor/destructor ***************/
	Model_Level2();
	~Model_Level2();

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