#include "model_3D.h"
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
Model_3D::Model_3D()
{
	minimap = NULL;
}

Model_3D::~Model_3D()
{
}

/*********** core functions ***************/
void Model_3D::Init()
{
	Model::Init();
	
	m_view_width = 180.f;
	m_view_height = 140.f;

	worldDimension.Set(1024, 800, 10);

	//perspective
	fovAngle = DEFAULT_FOV;
	FovRate = 1.f;
	storeFov = fovAngle;

	//light
	lightPos[0].Set(1000.f, 500.f, 0.f);
	lightPos[1].Set(0.f, 800.f, 0.f);

	//object
	InitObject();

	//minimap
	minimap = new MiniMap;
	minimap->SetBackground(MeshBuilder::GenerateMinimap("Minimap", Color(1, 1, 1), 1));
	minimap->GetBackground()->textureID[0] = LoadTGA("Image\\brick.tga");
	minimap->SetBorder(MeshBuilder::GenerateMinimapBorder("Minimap Border", Color(0, 1, 1)));
	minimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("Minimap avatar", Color(0, 1, 1)));
}

void Model_3D::InitObject()
{	
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
	parent->rotateObject(56, 1, 0, 0);
	parent->scaleObject(100, 50, 50);
	elementObject[1] = parent;

	obj_ptr = new Object;
	obj_ptr->Set("Child", Geometry::meshList[Geometry::GEO_CUBE], parent, false, true);
	obj_ptr->translateObject(0, 2, 0);	//translates 2 times of scale of parent
	obj_ptr->scaleObject(2, 1, 2);	//scales 2 times of parent
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


void Model_3D::Update(double dt, bool* myKeys)
{
	/* model update */
	Model::Update(dt, myKeys);

	/* Camera */
	camera.Update(dt, myKeys);

	/* element 0: follow camera pos */
	elementObject[0]->translate(camera.position);

	/* test object element 1: parent with child(element 2) test */
	if(myKeys[KEY_B])	//translate X dir 1o * dt speed
	{
		elementObject[1]->translateObject(50 * dt, 0, 0);
	}

	if(myKeys[KEY_V])	
	{
		elementObject[1]->scaleObject(1.1, 1.1, 1.1);
	}

	/* fov */
	UpdateFOV(dt, myKeys);
}

void Model_3D::UpdateLight(double dt, bool* myKeys, Light* light)
{
}

void Model_3D::Exit()
{
	Model::Exit();
}