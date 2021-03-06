#ifndef MODEL_LEVEL_H
#define MODEL_LEVEL_H
#include "Model.h"
#include "TriggerArea.h"

//model for level class
class Model_Level : public Model
{
protected:
/************ All Game object vectors ************/ 

	/* render all general(not a specific object) game objects */
	vector<GameObject*> goList;	
	
	/* render all collision boxes (debug purpose only pls REMOVE) */
	vector<Collision*> collisionList; 
	
	/* all item objects: not needed can use goList */
	vector<Item*> itemList;	
	
	/* all ui objects: Render in 2D screen */
	vector<UI_Object*> UI_List;	
	
	/* Triggering Objects: not needed can use goList */
	vector<TriggerObject*> triggerObject;

	static bool init_Already;	//init all parent class stuff already?
	bool initBasicsAlready;	//init child class basics already?

	StateManager::STATES state;
public:
/************ current level map ************/ 
	vector<Map*>* level_map;

/************* Map manager (Only one for all levels) *****************/
	static MapManager mapManager;

	Vector3 mapSize;	

/************ Game Objects  ************/ 

	/* Universal game objects */
	//player
	static Player* player;
	static float hero_Health;

	// UI objects
	/* In game */
	UI_Object main_UI_bar;	//main UI in game
	UI_Object puzzleMessage;	//UI for puzzle message	(Use this to customise, take
	UI_Object tutorialUI;	//UI for showing tutorial (Lvl 1 and 2 only)

	/* Rate of pressing the see puzzle msg */
	double puzzleMsgTimer;	
	double puzzleMsgTime;

	/* Rate of pressing invisbility */
	double inviTimer;
	double inviTime;

	// door/checkpoint 
	TriggerObject* door;
	TriggerObject* door_R;
	TriggerObject* door_P;
	TriggerObject* door_G;

	TriggerObject* staircase;
	static bool stopGame;
	bool doorUnlocked;
	bool doorRUnlocked;
	bool doorPUnlocked;
	bool doorGUnlocked;
	static bool haveFire;
	static double Timer;
	static double mapTimer;

	/* damage values here */
	const static int SPIKE_DMG = 10;

	/* Player stuff */
	Vector3 originalPos;	//original player position

	/* Common game objects */
	//Enemy
	Ogre* E_Ogre;
	Item* item;

	//puzzle
	PuzzleManager *puzzleManager;
	bool puzzleOpen;
	bool openTutorial;
	static bool playerWin;
	static bool playerDie;
	
	static bool goNextLevel;
	static bool goPreviousLevel;
	static bool goMainMenu;
	static bool restartLevel;

	/*********** constructor/destructor ***************/
	Model_Level();
	virtual ~Model_Level();

	/*********** core functions ***************/
	virtual void Init();
		void InitSprite();
		void InitStaticSprite();
		void InitMaps(); //Initialize maps (RP)

	virtual void Update(double dt, bool* myKeys, Vector3 mousePos, StateManager::STATES currentState);
	virtual void ClearLevel();
	virtual void Exit();

	/*********** getter/setter ***************/
	static bool NextLevel();	//whether can go to next level or not
	static bool PreviousLevel();
	static void setNextLevel(bool i);	//whether can go to next level or not
	static void setPreviousLevel(bool i);
	static bool MainMenu();
	static bool Restart();
	static bool WinGame();
	static bool LoseGame();

	StateManager::STATES getState();
	void setState(StateManager::STATES state);

	/********** Utilities *************/
	bool ReadFromFile(char* text);

	/*********** getter / setters ***************/
	vector<GameObject*>* getObject();
	vector<UI_Object*>* getUIList();
	vector<Item*>* getItemList();
	vector<Map*>* getLevelMap();
};

#endif