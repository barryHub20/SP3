#ifndef MODEL_H
#define MODEL_H
#include "GameObject.h"
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
public:
	/*********** constructor/destructor ***************/
	Model();
	virtual ~Model();

	/*********** core functions ***************/
	virtual void Init();
	void InitMesh();

	virtual void Update(double dt, bool* myKeys);
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
	Object* getObject(int index);
	unsigned short getViewWidth();
	unsigned short getViewHeight();
	Vector3 getWorldDimension();

	/************* Minimap *****************/
	MiniMap* minimap;

	//!! 2D Map Stuff should not be in Model (parent class), should be in child classes
	/************* TileMap *****************/
	MapManager *mapManager;
	Map* m_backgroundMap;
	Map* MapList[LEVEL_2 + 1];	//store the maps for each level
	float m_backgroundSpeed_Percent;	//background scrolls at % speed of normal scrolling speed

	/* constrain hero */
	int hero_Health;	//keeps track of hero health
	float constrain_PercentX;	//how much of screen you want to constrain
	Vector2 middlePos;	//pos of middle of screen X
	int constrain_DistX;	//how much far from middle then constrain

	/* side scrolling */
	int tileOffset_X, tileOffset_Y;
	Vector2 offset;	//offset X by how many pixels

	/******************** Game state ************************/
	GAME_STATE game_state;
	StateManager *stateManager;

	//player
	Player* player;

	// ================== Audio =================

	SoundManager *sfx_man;
};

#endif