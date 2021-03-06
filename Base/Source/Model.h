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
#include "Camera_2D.h"
#include "Mesh.h"
#include "Light.h"
#include "MiniMap.h"
#include "Object.h"
#include <vector>
#include "Player.h"
#include "StateManager.h"
#include "Inventory.h"
#include "TriggerObject.h"
#include "PuzzleManager.h"
using namespace std;

#include "SoundManager.h"

enum MODEL
{
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4,
	LEVEL_5,
	TOTAL_MODEL,
};

class Model
{
protected:
	static int model_count;	//how many model
	static int current_model;

	/** key pressed check **/
	double keyPressedTimer;
	double delayTime;	//everytime press a button, delay before register button press again
/********************** View size *****************************/
	static unsigned short m_view_width;	//camera view size X
	static unsigned short m_view_height;	//camera view size Y

	static unsigned short m_2D_view_width;
	static unsigned short m_2D_view_height;

/************* Camera *****************/
	Camera2D camera;

/************* fps *****************/
	static bool bLightEnabled;
	static float fps;

/************ lights ************/
	const static unsigned TOTAL_LIGHTS = 2;
	static Position lightPos[TOTAL_LIGHTS];

public:
	/*********** constructor/destructor ***************/
	Model();
	virtual ~Model();

	/*********** core functions ***************/
	virtual void Init();
	void InitMesh();

	virtual void Update(double dt, bool* myKeys, Vector3 mousePos);
	void UpdateOpenGL(double dt, bool* myKeys);
	virtual void Exit();

	/*********** getter / setters ***************/
	static bool getbLightEnabled();
	Camera2D* getCamera();
	static float getFPS();
	static Position getLightPos(int index);
	static unsigned short getViewWidth();
	static unsigned short getViewHeight();
	static unsigned short get2DViewWidth();
	static unsigned short get2DViewHeight();
	static int getModelCount();	//total model
	static void setCurrentModel(int i);
	static int getCurrentModel();	//current model

	/************** General Utilities *****************/
	bool getWordFromString(string& sentence, string& word, char min, char max, int& index);
	float stringTofloat(string& number);

	// ================== Audio =================
	static SoundManager *sfx_man;
};

#endif