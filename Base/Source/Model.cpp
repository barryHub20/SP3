#include "Model.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

/* 1) m_map is supposed to init in constructor of model 
   2) Make map reader such that if line length exceeds, do not read the remaining numbers from csv.	
   3) Render too much tile = lag
   4) xOffset uses int so jagged effect, use float next time
*/
const float Model::MIN_FOV = 1.f;
const float	Model::MAX_FOV = 100.f;
const float Model::DEFAULT_FOV = 45.f;

/*********** constructor/destructor ***************/
Model::Model() :
	mapManager(NULL)
{
}

Model::~Model()
{
	delete mapManager;
}

/*********** core functions ***************/
void Model::Init()
{
	/* game state setup */
	mapManager = new MapManager();
	stateManager = new StateManager();

	stateManager->ChangeState(StateManager::MAIN_MENU);

	/* Must Init */
	InitMesh();

	//if mode is 3D
	camera.Init(Vector3(0, 0, 0), Vector3(0, 0, -10), Vector3(0, 1, 0));

	hero_Health = 0;

	bLightEnabled = true;
}

void Model::InitMesh()
{
	Geometry::Init();
}

void Model::Update(double dt, bool* myKeys)
{
	/* openGL stuff */
	UpdateOpenGL(dt, myKeys);

	/* Sprite animation */
	for(std::vector<SpriteAnimation*>::iterator it = Geometry::animation.begin(); it != Geometry::animation.end(); ++it)
	{
		SpriteAnimation *go = (SpriteAnimation *)*it;
		go->Update(dt);
	}

	fps = (float)(1.f / dt);

	/* Framerate checker: if drop below 57, performance issues */
	if(fps <= 57.f)
		cout << "Framerate dropped to: " << fps << endl;
}

void Model::UpdateOpenGL(double dt, bool* myKeys)
{
	if(myKeys[KEY_1])
		glEnable(GL_CULL_FACE);
	if(myKeys[KEY_2])
		glDisable(GL_CULL_FACE);
	if(myKeys[KEY_3])
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(myKeys[KEY_4])
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Model::Exit()
{
	mapManager->CleanUp();
	Geometry::Exit();
}

void Model::UpdateFOV(double dt, bool* myKeys)
{
	//if zoom activated
	//	storeFov = fovAngle;
	static bool zoomActivate = false;
	static double zoomTime = 0.2f;
	static double zoomTimer = zoomTime;

	if(zoomTimer < zoomTime)
		zoomTimer += dt;

	/* Zooming */
	if(myKeys[KEY_RMOUSE])	//activate zoom
	{
		if(zoomTimer >= zoomTime)
		{
			if(!zoomActivate)
			{
				storeFov = fovAngle;
				fovAngle = DEFAULT_FOV;	//zoom starts at 45 deg.
				zoomActivate = true;
			}
			else
			{
				fovAngle = storeFov;
				zoomActivate = false;
			}
			zoomTimer = 0.0;
		}
	}

	/* manipulate fov angle */
	if(!zoomActivate)
	{
		if(myKeys[KEY_I] && fovAngle - FovRate >= DEFAULT_FOV) 
			fovAngle -= FovRate;
		if(myKeys[KEY_O] && fovAngle + FovRate <= MAX_FOV)
			fovAngle += FovRate;
	}
	else
	{
		if(myKeys[KEY_I] && fovAngle - FovRate >= MIN_FOV) 
			fovAngle -= FovRate;
		if(myKeys[KEY_O] && fovAngle + FovRate <= DEFAULT_FOV)
			fovAngle += FovRate;
	}
}

/*********** getter / setters ***************/
bool Model::getbLightEnabled(){return bLightEnabled;}
float Model::getFOV(){return fovAngle;}
Camera3* Model::getCamera(){return &camera;}
Vector3 Model::getWorldDimension(){return worldDimension;}
unsigned short Model::getViewWidth(){return m_view_width;}
unsigned short Model::getViewHeight(){return m_view_height;}

Position Model::getLightPos(int index)
{
	//if index exceeds
	if(index < 0 || index >= TOTAL_LIGHTS)
		return Position(0, 0, 0);
	else
		return lightPos[index];
}

vector<Object*>* Model::getObject(){return &elementObject;}

Object* Model::getObject(int index)
{
	return elementObject[index];
}
