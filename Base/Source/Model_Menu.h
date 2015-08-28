//#ifndef MODEL2D_H
//#define MODEL2D_H
//#include "Model.h"
//
//class Model_Menu : public Model
//{
//	/** key pressed check **/
//	double keyPressedTimer;
//	double delayTime;	//everytime press a button, delay before register button press again
//public:
///*********** constructor/destructor ***************/
//	Model_Menu();
//	~Model_Menu();
//
///*********** core functions ***************/
//	//Initialize
//	virtual void Init();
//	void InitTrigger();
//	void InitUI();
//	void InitObject();
//	void InitSprites();
//	void InitPuzzles();
//	void spawnItems();
//	void InitMaps(); //Initialize maps (RP)
//
//	//Update
//	virtual void Update(double dt, bool* myKeys, Vector3 mousePos);
//	void UpdateMainMenu(double dt, bool* myKeys, double mouse_x, double mouse_y);	//updating main menu
//	void UpdateGame(double dt, bool* myKeys);
//	void UpdateInstructions(double dt, bool* myKeys, double mouse_x, double mouse_y);
//	void UpdateEnemy(double dt);
//	void UpdateTraps(double dt, bool* myKeys);
//
//	void UpdateLight(double dt, bool* myKeys, Light* light);	//get light from view
//
//	virtual void Exit();
//
///********** save load functions *************/
//	bool ReadFromFile(char* text);
//};
//
//#endif