#ifndef MODEL2D_H
#define MODEL2D_H
#include "Model_Level.h"

class Model_2D : public Model_Level
{
public:
/*********** constructor/destructor ***************/
	Model_2D();
	~Model_2D();

/*********** core functions ***************/
	//Initialize
	virtual void Init();
	void InitTrigger();
	void InitUI();
	void InitObject();
	void InitSprites();
	void InitPuzzles();
	void InitMaps();
	void spawnItems();

	//Update
	virtual void Update(double dt, bool* myKeys, Vector3 mousePos);
		//void UpdateMainMenu(double dt, bool* myKeys, double mouse_x, double mouse_y);	//updating main menu
		void UpdateGame(double dt, bool* myKeys);
		//void UpdateInstructions(double dt, bool* myKeys, double mouse_x, double mouse_y);
		void UpdateEnemy(double dt);
		void UpdateTraps(double dt, bool* myKeys);


	virtual void Exit();
};

#endif