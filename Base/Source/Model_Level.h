#ifndef MODEL_LEVEL_H
#define MODEL_LEVEL_H
#include "Model.h"

//model for level class
class Model_Level : public Model
{
protected:
/************ All Game object vectors ************/ 
	vector<GameObject*> goList;	//render all general(not a specific object) game objects
	vector<Collision*> collisionList; //render all collision boxes (debug purpose only pls remove)
	vector<Item*> itemList;	//all item objects
	vector<UI_Object*> UI_List;	//all ui objects
	static bool init_Already;
public:
/************ current level map ************/ 
	vector<Map*>* level_map;

/************* Map manager (Only one for all levels) *****************/
	static MapManager mapManager;

/************ Gmae Objects ************/ 

	//player
	static Player* player;
	float hero_Health;

	//Triggering Objects
	vector<TriggerObject*> triggerObject;

	// UI 
	UI_Object cursor;
	UI_Object start_Game;
	UI_Object instruction;
	UI_Object go_back;

	// door/checkpoint 
	TriggerObject* door;
	bool stopGame;
	bool doorUnlocked;
	bool haveFire;
	double Timer;
	double mapTimer;
	TriggerObject* staircase;

	//objects
	Ogre* E_Ogre;
	Item* item;

	// array of static objects (non-moving)
	StaticObject* obj_arr[10];

	//puzzle
	PuzzleManager *puzzleManager;
	bool puzzleOpen;
	
	static bool goNextLevel;
	static bool goPreviousLevel;

	/*********** constructor/destructor ***************/
	Model_Level();
	virtual ~Model_Level();

	/*********** core functions ***************/
	virtual void Init();
		//virtual void InitTrigger();
		//virtual void InitUI();
		//virtual void InitObject();
		//virtual void InitSprites();
		//virtual void InitPuzzles();
		//virtual void spawnItems();
		void InitMaps(); //Initialize maps (RP)

	virtual void Update(double dt, bool* myKeys, Vector3 mousePos);
		/*virtual void UpdateGame(double dt, bool* myKeys);
		virtual void UpdateEnemy(double dt);
		virtual void UpdateTraps(double dt, bool* myKeys);*/

	virtual void Exit();

	/*********** getter/setter ***************/
	static bool NextLevel();	//whether can go to next level or not
	static bool PreviousLevel();
	static void setNextLevel(bool i);	//whether can go to next level or not
	static void setPreviousLevel(bool i);

	/********** Utilities *************/
	bool ReadFromFile(char* text);

	/*********** getter / setters ***************/
	vector<GameObject*>* getObject();
	vector<UI_Object*>* getUIList();
	vector<Item*>* getItemList();
	vector<Map*>* getLevelMap();
};

#endif