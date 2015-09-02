#ifndef Model_Screen_H
#define Model_Screen_H
#include "Model_Level.h"

class Model_Screen : public Model_Level
{
	//UI
	/* Main menu */
	static UI_Object cursor;
	static UI_Object start_Game;
	static UI_Object mainmenu;
	static UI_Object instructionscreen;
	static UI_Object instruction;
	static UI_Object go_back;
	static UI_Object winGame;
	static UI_Object loseGame;
	static UI_Object restart;
	static UI_Object backmenu;
public:
	/*********** constructor/destructor ***************/
	Model_Screen();
	~Model_Screen();

	/*********** core functions ***************/
	//Initialize
	virtual void Init();

	UI_Object* wordBox;

	//Update
	virtual void Update(double dt, bool* myKeys, Vector3 mousePos, StateManager::STATES currentState);
	void UpdateMainMenu(double dt, bool* myKeys, double mouse_x, double mouse_y);	//updating main menu
	void UpdateInstructions(double dt, bool* myKeys, double mouse_x, double mouse_y);
	void UpdateWinScreen(double dt, bool* myKeys, double mouse_x, double mouse_y);
	void UpdateLoseScreen(double dt, bool* myKeys, double mouse_x, double mouse_y);

	virtual void Exit();

	bool pauseGame;
	bool wasdFinish;
	double pressTimer;
};

#endif