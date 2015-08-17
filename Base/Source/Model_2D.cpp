#include "model_2D.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Controller.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadHmap.h"
#include "SpriteAnimation.h"
#include <sstream>

/*********** constructor/destructor ***************/
Model_2D::Model_2D()
{
	minimap = NULL;
}

Model_2D::~Model_2D()
{
}

/*********** core functions ***************/
void Model_2D::Init()
{
	Model::Init();
	
	InitMaps();
	mapManager->SetMap(0);
	mapManager->GetCurrentMap()->LoadBBox();

	/* Coord of screen */
	m_view_width = 1024.f;
	m_view_height = 800.f;

	/* World boundaries */
	worldDimension.Set(800, 800, 100);

	//perspective
	fovAngle = DEFAULT_FOV;
	FovRate = 1.f;
	storeFov = fovAngle;

	//light
	lightPos[0].Set(1000.f, 500.f, 0.f);
	lightPos[1].Set(0.f, 800.f, 0.f);

	//object
	InitObject();
}

void Model_2D::InitObject()
{	
	/** Default pos : 0, 0, 0
		Default scale: 1, 1, 1
	**/

	Object* obj_ptr;

	/* resize vector to contain 4 spaces */
	elementObject.resize(4);

	//FOLLOW CAMERA OBJECT
	obj_ptr = new Object;
	obj_ptr->Set("character", Geometry::meshList[Geometry::GEO_CUBE], NULL, false, true);
	obj_ptr->scaleObject(10);
	elementObject[0] = obj_ptr;

	/*********** TEST 1: child translates to above parent Obj 1 and 2 ***********/
	Object* parent = NULL;
	parent = new Object;
	parent->Set("Parent", Geometry::meshList[Geometry::GEO_CUBE], NULL, false, true);
	parent->translateObject(m_view_width * 0.5f, m_view_height * 0.5f, 0);
	//parent->rotateObject(56, 1, 0, 0);
	parent->scaleObject(50, 20, 50);
	elementObject[1] = parent;

	obj_ptr = new Object;
	obj_ptr->Set("Child", Geometry::meshList[Geometry::GEO_CUBE], parent, false, true);
	obj_ptr->translateObject(0, 1.5f, 0);	//translates 2 times of scale of parent
	//obj_ptr->scaleObject(2, 1, 2);	//scales 2 times of parent
	elementObject[2] = obj_ptr;


	/* Axes */
	obj_ptr = new Object;
	obj_ptr->Set("Axes", Geometry::meshList[Geometry::GEO_AXES], NULL, false, true);
	elementObject[3] = obj_ptr;

	/** init **/
	for(std::vector<Object*>::iterator it = elementObject.begin(); it != elementObject.end(); ++it)
	{
		Object *go = (Object *)*it;
		go->Init();
	}
}

void Model_2D::InitMaps()
{
	mapManager->CreateMap(32, 25, 32, "Image//Map//MapDesign.csv");
	mapManager->CreateMap(32, 25, 32, "Image//Map//MapDesign_lvl1.csv");
	mapManager->CreateMap(32, 25, 32, "Image//Map//MapDesign_lvl2.csv");
}

void Model_2D::Update(double dt, bool* myKeys)
{
	/* model update */
	Model::Update(dt, myKeys);

	/* Camera */
	//camera.Update(dt, myKeys);

	/* control character Test */
	//cout << elementObject[0]->getBbox()->collideArea.collideSide;

	if(myKeys[KEY_W])
	{
		elementObject[0]->translateObject(0, 50 * dt, 0);
	}

	if(myKeys[KEY_A])	
	{
		elementObject[0]->translateObject(-50 * dt, 0, 0);
	}

	if(myKeys[KEY_S])	
	{
		elementObject[0]->translateObject(0, -50 * dt, 0);
	}

	if(myKeys[KEY_D])	
	{
		elementObject[0]->translateObject(50 * dt, 0, 0);
	}

	//Key B to move to next map (RP)
	static bool ButtonBState = false;
	if (!ButtonBState && myKeys[KEY_B])
	{
		ButtonBState = true;
		std::cout << "BBUTTON DOWN" << std::endl;
		mapManager->ChangeNextMap();
	}
	else if (ButtonBState && !(myKeys[KEY_B]))
	{
		ButtonBState = false;
		std::cout << "BBUTTON UP" << std::endl;
	}

	/* Check collision */
	elementObject[0]->checkCollision(*elementObject[1]);
	elementObject[0]->checkCollision(*elementObject[2]);
	elementObject[0]->getBbox()->Reset();	//RMB to reset

	/* fov */
	//UpdateFOV(dt, myKeys);
}

void Model_2D::UpdateLight(double dt, bool* myKeys, Light* light)
{
}

void Model_2D::Exit()
{
	Model::Exit();
}