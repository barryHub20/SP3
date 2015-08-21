#ifndef MODEL2D_H
#define MODEL2D_H
#include "Model.h"

class Model_2D : public Model
{
	/* player */
	Player* player;
	Enemy* enemy;
	Inventory* inventory;
	Item item;
	/* array of static objects (non-moving) */
	StaticObject* obj_arr[10];

	/** key pressed check **/
	double keyPressedTimer;
	double delayTime;	//everytime press a button, delay before register button press again
public:
/*********** constructor/destructor ***************/
	Model_2D();
	~Model_2D();

/*********** core functions ***************/
	//Initialize
	virtual void Init();
	void InitObject();
	void InitSprites();
	void InitMaps(); //Initialize maps (RP)

	//Update
	virtual void Update(double dt, bool* myKeys);
	void UpdateMainMenu(double dt, bool* myKeys, double mouse_x, double mouse_y);	//updating main menu
	void UpdateGame(double dt, bool* myKeys);
	void UpdateInstructions(double dt, bool* myKeys, double mouse_x, double mouse_y);
	void UpdateEnemy(double dt);

	void UpdateLight(double dt, bool* myKeys, Light* light);	//get light from view

	virtual void Exit();

/********** save load functions *************/
	bool ReadFromFile(char* text);
};

#endif