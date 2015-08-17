#include "Model_Physics.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Controller.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadHmap.h"
#include "SpriteAnimation.h"
#include <sstream>

/*********** constructor/destructor ***************/
Model_Physics::Model_Physics()
{
	minimap = NULL;
}

Model_Physics::~Model_Physics()
{
}

/*********** core functions ***************/
void Model_Physics::Init()
{
	Model::Init();
	
	m_view_width = 180.f;
	m_view_height = 120.f;

	worldDimension.Set(800, 600, 10);

	//perspective
	fovAngle = DEFAULT_FOV;
	FovRate = 1.f;
	storeFov = fovAngle;

	//light
	lightPos[0].Set(500.f, 800.f, 200.f);
	lightPos[1].Set(0.f, 8000.f, 0.f);

	//object
	InitObject();

	//billboard
	InitBillboard();


	//minimap
	minimap = new MiniMap;
	minimap->SetBackground(MeshBuilder::GenerateMinimap("Minimap", Color(1, 1, 1), 1));
	minimap->GetBackground()->textureID[0] = LoadTGA("Image\\brick.tga");
	minimap->SetBorder(MeshBuilder::GenerateMinimapBorder("Minimap Border", Color(0, 1, 1)));
	minimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("Minimap avatar", Color(0, 1, 1)));
}

void Model_Physics::InitSnow()
{
	Particle* particle_ptr;
	lengthFromPlayer = 1000;	//4 sides: front back left right
	howManyParticles = 10;	//how many particles per length
	initialY = 500;	//fixed Y spawning pos
	pos_offset = camera.position;
	initial_pos = camera.position;

	//set index
	start_Index = elementObject.size();
	end_Index = start_Index;

	float x = camera.position.x - lengthFromPlayer;
	float z = camera.position.z - lengthFromPlayer;
	float y_Offset = 300.f;
	float y = initialY;
	float distInBtw = lengthFromPlayer / howManyParticles;	//100 / 50 = 2

	for(int i = 0; i < howManyParticles * 2; ++i)
	{
		for(int j = 0; j < howManyParticles * 2; ++j)
		{
			particle_ptr = new Particle;

			/* x dir */
			particle_ptr->Set("Snow", Geometry::meshList[Geometry::GEO_SNOWFLAKE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, Particle::SNOW, camera.position, Math::RandFloatMinMax(5, 9));
			particle_ptr->scaleObject(3, 3, 3);
			particle_ptr->translateObject(Vector3(x, y, z));
			x += distInBtw;

			y = Math::RandFloatMinMax(initialY - y_Offset, initialY + y_Offset);

			elementObject.push_back(particle_ptr);
			end_Index++;
		}
		z += distInBtw;
		x = camera.position.x - lengthFromPlayer;
	}
}

void Model_Physics::InitObject()
{	
	Object* obj_ptr;

	//elementObject.resize(20);

	/* Collision test objects */
	//FOLLOW CAMERA OBJECT
	obj_ptr = new Object;
	obj_ptr->Set("character", Geometry::meshList[Geometry::GEO_CUBE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, true);
	obj_ptr->scaleObject(5);
	elementObject.push_back(obj_ptr);

	//obj_ptr = new Object;
	//obj_ptr->Set("character", Geometry::meshList[Geometry::GEO_CUBE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, true);
	//obj_ptr->translateObject(-75, 75, 0);
	//obj_ptr->scaleObject(25);
	//elementObject.push_back(obj_ptr);

	//obj_ptr = new Object;
	//obj_ptr->Set("character", Geometry::meshList[Geometry::GEO_CUBE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, true);
	//obj_ptr->translateObject(-74, 100, 0);
	//obj_ptr->scaleObject(50, 25, 25);
	//elementObject.push_back(obj_ptr);

	//obj_ptr = new Object;
	//obj_ptr->Set("character", Geometry::meshList[Geometry::GEO_CUBE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, true);
	//obj_ptr->translateObject(-74, 125, 0);
	//obj_ptr->scaleObject(25, 25, 35);
	//elementObject.push_back(obj_ptr);

	//parent
	obj_ptr = new Object;
	obj_ptr->Set("character", Geometry::meshList[Geometry::GEO_CUBE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, true);
	obj_ptr->scaleObject(5, 5, 5);
	elementObject.push_back(obj_ptr);

	//chilkd
	obj_ptr = new Object;
	obj_ptr->Set("character", Geometry::meshList[Geometry::GEO_CUBE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, true);
	obj_ptr->AddParent(elementObject[1]);
	elementObject.push_back(obj_ptr);

	obj_ptr = new Object;
	obj_ptr->Set("skybox front", Geometry::meshList[Geometry::GEO_FRONT], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);
	obj_ptr->translateObject(0, 0, 5884);
	obj_ptr->rotateObject(180, 0, 1, 0);
	obj_ptr->scaleObject(12000, 12000, 12000);
	elementObject.push_back(obj_ptr);

	obj_ptr = new Object;
	obj_ptr->Set("skybox right", Geometry::meshList[Geometry::GEO_RIGHT], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);
	obj_ptr->translateObject(-5884, 0, 0);
	obj_ptr->rotateObject(90, 0, 1, 0);
	obj_ptr->scaleObject(12000, 12000, 12000);
	elementObject.push_back(obj_ptr);

	obj_ptr = new Object;
	obj_ptr->Set("skybox left", Geometry::meshList[Geometry::GEO_LEFT], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);
	obj_ptr->translateObject(5884, 0, 0);
	obj_ptr->rotateObject(-90, 0, 1, 0);
	obj_ptr->scaleObject(12000, 12000, 12000);
	elementObject.push_back(obj_ptr);

	obj_ptr = new Object;
	obj_ptr->Set("skybox back", Geometry::meshList[Geometry::GEO_BACK], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);
	obj_ptr->translateObject(0, 0, -5884);
	obj_ptr->scaleObject(12000, 12000, 12000);
	elementObject.push_back(obj_ptr);

	obj_ptr = new Object;
	obj_ptr->Set("skybox top", Geometry::meshList[Geometry::GEO_TOP], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);	//rotate to face top by X
	obj_ptr->translateObject(0, 5884, 0);
	obj_ptr->rotateObject(90, 1, 0, 0);
	obj_ptr->rotateObject(180, 0, 0, 1);
	obj_ptr->scaleObject(12000, 12000, 12000);
	elementObject.push_back(obj_ptr);

	obj_ptr = new Object;
	obj_ptr->Set("skybox bottom", Geometry::meshList[Geometry::GEO_BOTTOM], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);	//rotate to face top by X
	obj_ptr->translateObject(0, -5884, 0);
	obj_ptr->rotateObject(-90, 1, 0, 0);
	obj_ptr->scaleObject(12000, 12000, 12000);
	elementObject.push_back(obj_ptr);

	/* building */
	float xPos = -750;
	for(int i = 0; i < 3; ++i)
	{
		obj_ptr = new Object;
		obj_ptr->Set("derelict building 01", Geometry::meshList[Geometry::GEO_DERELICT_BUILDING_01], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, true, true);	//rotate to face top by X
		obj_ptr->translateObject(xPos, 50, -25);
		//obj_ptr->rotateObject(-90, 1, 0, 0);
		obj_ptr->scaleObject(25, 25, 25);
		elementObject.push_back(obj_ptr);

		xPos += 400.f;
	}

	//REMOVE
	//obj_ptr = new Object;
	//obj_ptr->Set("test cube", Geometry::meshList[Geometry::GEO_CUBE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, true, true);	//rotate to face top by X
	//obj_ptr->translateObject(0, 0, 0);
	////obj_ptr->rotateObject(-90, 1, 0, 0);
	//obj_ptr->scaleObject(10, 100, 10);
	//elementObject.push_back(obj_ptr);

	/* police car */
	//7
	float car_distX = 350;
	for(int j = 0; j < 5; ++j)
	{
		Object* parent;
		parent = new Object;
		parent->Set("police car main", Geometry::meshList[Geometry::GEO_CAR_MAIN_PART], Vector3(0.2, 0.2, 0.2), Vector3(0, 0, 0), NULL, true, true);
		parent->translateObject(car_distX, 10, 300);
		parent->rotateObject(30, 0, 1, 0);
		parent->scaleObject(100);
		elementObject.push_back(parent);

		obj_ptr = new Object;
		obj_ptr->Set("police car glass", Geometry::meshList[Geometry::GEO_CAR_GLASS], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, true, true);
		//obj_ptr->scaleObject(100);
		obj_ptr->AddParent(parent);
		elementObject.push_back(obj_ptr);

		obj_ptr = new Object;
		obj_ptr->Set("police car siren", Geometry::meshList[Geometry::GEO_CAR_SIREN], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, true, true);
		//obj_ptr->scaleObject(100);
		obj_ptr->AddParent(parent);
		elementObject.push_back(obj_ptr);

		car_distX -= 150;
	}

	/* tank */
	/*elementObject[10]->Set("T90 main", Geometry::meshList[Geometry::GEO_T90_MAIN], Vector3(30, 30, 30), Vector3(100, 20, 100), NULL, true);
	elementObject[10]->translateObject(0, 12, 0);
	elementObject[11]->Set("T90 turrnet", Geometry::meshList[Geometry::GEO_T90_TURRNET], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, true);
	elementObject[11]->rotateObject(38, 0, 1, 0);
	elementObject[11]->AddParent(elementObject[10]);	*/

	obj_ptr = new Object;
	obj_ptr->Set("ground", Geometry::meshList[Geometry::GEO_TERRAIN], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, true, true);
	obj_ptr->scaleObject(2000, 100, 2000);
	elementObject.push_back(obj_ptr);

	//snow
	InitSnow();

	/** init **/
	for(std::vector<Object*>::iterator it = elementObject.begin(); it != elementObject.end(); ++it)
	{
			Object *go = (Object *)*it;
			go->Init();
	}
}

void Model_Physics::InitBillboard()
{
	/** SET **/
	Billboard* obj_ptr;
	for(int i = 0; i < 9; ++i)
	{
		obj_ptr = new Billboard;
		billboardObject.push_back(obj_ptr);
	}

	float xPos = 700.f;
	float zPos = 717.f;
	float yPos = 60.f;
	bool flipZ = false;
	for(int i = 0; i < 9; ++i)
	{
		billboardObject[i]->Set("tree", Geometry::meshList[Geometry::GEO_BILLBOARD_TREE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false);	//rotate to face top by X
		billboardObject[i]->translateObject(xPos, yPos, zPos);
		billboardObject[i]->scaleObject(100, 100, 100);
		xPos -= 200.f;
		
		if(i > 5)	//hardcode: no choice
			yPos += 40.f;

		if(!flipZ)
		{
			flipZ = true;
			zPos -= 50.f;
		}
		else
		{
			flipZ = false;
			zPos += 50.f;
		}
	}
}

void Model_Physics::Update(double dt, bool* myKeys)
{
	/* model update */
	Model::Update(dt, myKeys);

	/*std::vector<Object*>::iterator it = elementObject.begin();
	Object* obj_ptr;*/

	/* element 0: follow camera pos */
	elementObject[0]->translate(camera.position);

	///* test object element 1: parent with child(element 2) test */
	//obj_ptr = (Object*)*(it + 1);
	//if(myKeys['B'])
	//{
	//	elementObject[1]->translateObject(10 * dt, 0, 0);
	//}

	//if(myKeys['V'])
	//{
	//	elementObject[1]->scaleObject(10 * dt, 10 * dt, 10 * dt);
	//}

	//light
	if(myKeys[KEY_5])	//forward
	{
		lightPos[0].x += 200 * dt;
	}
	if(myKeys[KEY_6])	//backward
	{
		lightPos[0].x -= 200 * dt;
	}

	/* check collision TEST */
	//elementObject[0]->checkCollision(*elementObject[3]);
	//elementObject[0]->checkCollision(*elementObject[1]);
	//elementObject[0]->checkCollision(*elementObject[2]);
	//elementObject[0]->getBbox()->Reset();	//RMB to reset

	//cout << elementObject[0]->getPosition() << endl;
	/* fov */
	UpdateFOV(dt, myKeys);

	/* billboard */
	UpdateBillboard();

	/* snow */
	UpdateSnow(dt);

	/*for(std::vector<SpriteAnimation*>::iterator it = Geometry::animation.begin(); it != Geometry::animation.end(); ++it)
	{
		SpriteAnimation *go = (SpriteAnimation *)*it;
		go->Update(dt);
	}*/
}

void Model_Physics::UpdateSnow(double dt)
{
	for(int i = start_Index; i < end_Index; ++i)
	{
		elementObject[i]->Update(dt, camera.position);
	}
}

void Model_Physics::UpdateBillboard()
{
	for(std::vector<Billboard*>::iterator it = billboardObject.begin(); it != billboardObject.end(); ++it)
	{
		Billboard *go = (Billboard *)*it;
		go->Update(camera);
	}
}

void Model_Physics::UpdateLight(double dt, bool* myKeys, Light* light)
{
}

void Model_Physics::Exit()
{
	Model::Exit();
}