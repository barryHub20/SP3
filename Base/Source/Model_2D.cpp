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
	/** Setup game object */
	test_obj = new GameObject(Vector3(1, 1, 1), Vector3(50, 50, 1), Vector3(1, 0, 0), 10, true);
	elementObject.push_back(test_obj->getObject());

	/** Set up player **/
	player = new Player(Vector3(1, 1, 1), Vector3(50, 50, 1), Vector3(1, 0, 0), 10, true);
	elementObject.push_back(player->getObject());


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
	
	if (stateManager->GetState() == stateManager->MAIN_MENU) // Atm, a placeholder title screen with standard "Press Start" behaviour.
	{
		if(myKeys[KEY_SPACE])
		{
			stateManager->ChangeState(stateManager->GAME);
		}

		if(myKeys[KEY_I])
		{
			stateManager->ChangeState(stateManager->INSTRUCTION);
		}
	}

	if (stateManager->GetState() == stateManager->INSTRUCTION)
	{
		if(myKeys[KEY_I])
		{
			stateManager->ChangeState(stateManager->MAIN_MENU);
		}
	}

	if (stateManager->GetState() == stateManager->GAME)		// Game objects will only be 'active'/controllable when in the GAME state.
	{
		//->Update();	//RMB to reset
		/* Update player */
		player->Update(dt, myKeys);
	}

	//Key B to move to next map (RP)
	static bool ButtonBState = false;
	if (!ButtonBState && myKeys[KEY_B])
	{
		ButtonBState = true;
		std::cout << "BBUTTON DOWN" << std::endl;
		//stateManager->ChangeState(StateManager::MAIN_MENU);
		mapManager->ChangeNextMap();
	}
	else if (ButtonBState && !(myKeys[KEY_B]))
	{
		ButtonBState = false;
		std::cout << "BBUTTON UP" << std::endl;
	}

	//State
	if (stateManager->isTransition())
	{
		stateManager->UpdateTransitionTime(dt);
	}
}

void Model_2D::UpdateLight(double dt, bool* myKeys, Light* light)
{
}

void Model_2D::Exit()
{
	Model::Exit();
}