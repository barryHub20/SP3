#ifndef MODEL_LEVEL1_H
#define MODEL_LEVEL1_H
#include "Model_Level.h"
#include "GameObject.h"

class Model_Level1 : public Model_Level
{
public:
	/*********** constructor/destructor ***************/
	Model_Level1();
	~Model_Level1();

	enum TUTORIAL_STATE
	{
		TUTORIAL_1,
		TUTORIAL_2,
		TUTORIAL_3,
		TUTORIAL_4,
		TUTORIAL_5,
		TOTAL_TUTORIAL,
	};


	/*********** core functions ***************/
	//Initialize
	virtual void Init();
	void InitTrigger();
	void InitUI();
	void InitObject();
	void InitPuzzles();
	void spawnItems();

	UI_Object* wordBox;
	TUTORIAL_STATE tutorial_state; //enum to go through the tutorial.
	TriggerArea activateTutorial; //to start the tutorial.
	Collision tutorial2Area; //Collision box to see where the trigger area is.
	Collision tutorial3Area;
	Collision tutorial4Area;
	Collision tutorial5Area;

	//Update
	virtual void Update(double dt, bool* myKeys, Vector3 mousePos, StateManager::STATES currentState);
		//void UpdateMainMenu(double dt, bool* myKeys, double mouse_x, double mouse_y);	//updating main menu
		void UpdateGame(double dt, bool* myKeys);
		//void UpdateInstructions(double dt, bool* myKeys, double mouse_x, double mouse_y);
		void UpdateTutorial(double dt, bool* myKeys);
		void UpdateTraps(double dt, bool* myKeys);
		void UpdateEnemy(double dt);

	virtual void Exit();

	bool pauseGame;
	bool wasdFinish;
	double pressTimer;
};

#endif