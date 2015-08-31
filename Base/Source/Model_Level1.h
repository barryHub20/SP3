#ifndef MODEL_LEVEL1_H
#define MODEL_LEVEL1_H
#include "Model_Level.h"

class Model_Level1 : public Model_Level
{
public:
/*********** constructor/destructor ***************/
	Model_Level1();
	~Model_Level1();

/*********** core functions ***************/
	//Initialize
	virtual void Init();
	void InitTrigger();
	void InitUI();
	void InitObject();
	void InitPuzzles();
	void spawnItems();

	UI_Object* wordBox;

	//Update
	virtual void Update(double dt, bool* myKeys, Vector3 mousePos);
		void UpdateMainMenu(double dt, bool* myKeys, double mouse_x, double mouse_y);	//updating main menu
		void UpdateGame(double dt, bool* myKeys);
		void UpdateInstructions(double dt, bool* myKeys, double mouse_x, double mouse_y);
		void UpdateTutorial(double dt, bool* myKeys);
		void UpdateTraps(double dt, bool* myKeys);


	virtual void Exit();

	bool pauseGame;
	bool wasdFinish;
	double pressTimer;
};

#endif