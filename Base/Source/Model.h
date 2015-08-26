#ifndef MODEL_H
#define MODEL_H
#include "GameObject.h"
#include "UI_Object.h"
#include "StaticObject.h"
#include "Enemy.h"
#include "Ogre.h"
#include "Map.h"
#include "MapManager.h"
#include "MeshList.h"
#include "MatrixStack.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "Light.h"
#include "MiniMap.h"
#include "Object.h"
#include <vector>
#include "Player.h"
#include "StateManager.h"
#include "Inventory.h"
#include "TriggerObject.h"
using namespace std;

#include "SoundManager.h"

class Model
{
public:
/*********** State ***************/
	enum GAME_STATE
	{
		/* Levels */
		LEVEL_1 = 0,
		LEVEL_2,

		/* Other states */
		WIN,

		TOTAL_STATE,
	};

/************* misc *****************/
	const static float MIN_FOV;
	const static float	MAX_FOV;
	const static float DEFAULT_FOV;
	float storeFov;	//store the last used fov( >= 45 deg.) if zoom activated, once zoom deactivated switch back to storeFov
	float FovRate;	//how much to increase/decrease

protected:
/********************** View size *****************************/
	unsigned short m_view_width;	//camera view size X
	unsigned short m_view_height;	//camera view size Y

	unsigned short m_2D_view_width;
	unsigned short m_2D_view_height;

	Vector3 worldDimension;	//max dimemsion for in-game world (eg. 1000 by 1000 by 1000 world space)

	/************* Camera *****************/
	Camera3 camera;
	float fovAngle;	//this angle is used for fov

	/************* Light *****************/
	bool bLightEnabled;
	float fps;

	/************ lights ************/
	const static unsigned TOTAL_LIGHTS = 2;
	Position lightPos[TOTAL_LIGHTS];

	/************ Game objects ************/ 
	vector<GameObject*> goList;	//render all in-game objects
	vector<Collision*> collisionList;	//render all collision boxes (debug purpose only pls remove)
	vector<Item*> itemList;	//all items
	vector<UI_Object*> UI_List;	//all ui stuff
public:
	/*********** constructor/destructor ***************/
	Model();
	virtual ~Model();

	/*********** core functions ***************/
	virtual void Init();
	void InitMesh();

	virtual void Update(double dt, bool* myKeys, Vector3 mousePos);
	void UpdateOpenGL(double dt, bool* myKeys);
	void UpdateFOV(double dt, bool* myKeys);

	virtual void Exit();


	/*********** getter / setters ***************/
	bool getbLightEnabled();
	float getFOV();
	Camera3* getCamera();
	float getFPS();
	Position getLightPos(int index);
	vector<GameObject*>* getObject();
	vector<UI_Object*>* getUIList();
	Object* getObject(int index);
	unsigned short getViewWidth();
	unsigned short getViewHeight();
	unsigned short get2DViewWidth();
	unsigned short get2DViewHeight();
	Vector3 getWorldDimension();

	/************* Minimap *****************/
	MiniMap* minimap;

	//!! 2D Map Stuff should not be in Model (parent class), should be in child classes
	/************* TileMap *****************/
	MapManager *mapManager;
	Map* m_backgroundMap;
	Map* MapList[LEVEL_2 + 1];	//store the maps for each level

	/******************** Game state ************************/
	GAME_STATE game_state;
	StateManager *stateManager;

	//player
	Player* player;
	float hero_Health;

	//Triggering Objects
	TriggerObject* triggerObject;

	/* UI */
	UI_Object cursor;
	UI_Object start_Game;
	UI_Object instruction;
	UI_Object go_back;

	/* door/checkpoint */
	TriggerObject* door;
	bool doorUnlocked;
	TriggerObject* staircase;

	/* objects */
	Ogre* E_Ogre;
	Item* item;
	/* array of static objects (non-moving) */
	StaticObject* obj_arr[10];

	// ================== Audio =================
	SoundManager *sfx_man;
};

#endif