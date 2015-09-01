#include "Model_Screen.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Controller.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadHmap.h"
#include "SpriteAnimation.h"
#include <sstream>

#include "StaticObject.h"
//(tip) If create bullet, bullet class has a static TRS so that TRS update with current bullet pos

/*********** constructor/destructor ***************/
Model_Screen::Model_Screen()
{

}

Model_Screen::~Model_Screen()
{
}

/*********** core functions ***************/
void Model_Screen::Init()
{
	if (!initBasicsAlready)
	{
		initBasicsAlready = true;
		Model_Level::Init();

		/* timer and delay time */
		delayTime = 0.3;
		keyPressedTimer = delayTime;

		stopGame = false;
		mapTimer = 0;
		openTutorial = false;
		pauseGame = true;
		pressTimer = 0;

		cursor.Init(Geometry::meshList[Geometry::GEO_CURSOR],
			Vector3(m_2D_view_width * 0.5f, 0, 1.95f), Vector3(10, 10, 1),
			"", UI_Object::MOUSE_CURSOR, true);
		UI_List.push_back(&cursor);

		start_Game.Init(Geometry::meshList[Geometry::GEO_STARTGAME],
			Vector3(m_2D_view_width * 0.5f, 50, 1.95f), Vector3(40, 20, 1),
			"", UI_Object::STARTGAME, true);
		UI_List.push_back(&start_Game);

		instruction.Init(Geometry::meshList[Geometry::GEO_INSTRUCTIONS],
			Vector3(m_2D_view_width * 0.5f, 30, 1.95f), Vector3(40, 20, 1),
			"", UI_Object::INSTRUCTIONS, true);
		UI_List.push_back(&instruction);

		restart.Init(Geometry::meshList[Geometry::GEO_TRYAGAIN],
			Vector3(m_2D_view_width * 0.5f, 60, 1.95f), Vector3(30, 10, 1),
			"", UI_Object::RESTART, true);
		UI_List.push_back(&restart);

		backmenu.Init(Geometry::meshList[Geometry::GEO_RETURN],
			Vector3(m_2D_view_width * 0.5f, 40, 1.95f), Vector3(30, 10, 1),
			"", UI_Object::BACKMAIN, true);
		UI_List.push_back(&backmenu);

		winGame.Init(Geometry::meshList[Geometry::GEO_WINSCREEN],
			Vector3(m_2D_view_width * 0.5f, 60, 1.95f), Vector3(170, 120, 1),
			"", UI_Object::WIN, true);
		UI_List.push_back(&winGame);

		loseGame.Init(Geometry::meshList[Geometry::GEO_LOSESCREEN],
			Vector3(m_2D_view_width * 0.5f, 60, 1.95f), Vector3(170, 120, 1),
			"", UI_Object::LOSE, true);
		UI_List.push_back(&loseGame);

		state = StateManager::MAIN_MENU;
	}

	/* set bounds so camera spawns at correct place each time reenter this level */
	camera.SetBound(player->getPosition());
}

void Model_Screen::Update(double dt, bool* myKeys, Vector3 mousePos, StateManager::STATES currentState)
{
	/* parent class update */
	state = currentState;
	Model_Level::Update(dt, myKeys, mousePos, currentState);

	if (keyPressedTimer < delayTime)
		keyPressedTimer += dt;
	pressTimer += dt;
	/* Update based on states */
	switch (state)
	{
	case StateManager::MAIN_MENU:
		UpdateMainMenu(dt, myKeys, mousePos.x, mousePos.y);
		break;
	case StateManager::INSTRUCTION:
		UpdateInstructions(dt, myKeys, mousePos.x, mousePos.y);
		break;
	case StateManager::GAME:
		//UpdateGame(dt, myKeys);
		goNextLevel = true;
		setNextLevel(goNextLevel);
		break;
	case StateManager::WIN_SCREEN:
		UpdateWinScreen(dt, myKeys, mousePos.x, mousePos.y);
		break;
	case StateManager::LOSE_SCREEN:
		UpdateLoseScreen(dt, myKeys, mousePos.x, mousePos.y);
		break;

	case StateManager::TRANSITION:
		break;
	}
	cout << state << endl;
}

void Model_Screen::UpdateInstructions(double dt, bool* myKeys, double mouse_x, double mouse_y)
{
	/* Update cursor */
	cursor.Follow(mouse_x, mouse_y);	//hard coded console height

										/* Check collide */
	if (cursor.QuickAABBDetection(&go_back) && myKeys[KEY_LMOUSE])	//go back to main menu
	{
		go_back.SetActive(false);
		start_Game.SetActive(true);
		instruction.SetActive(true);
		state = StateManager::MAIN_MENU;
		//Model_Level::stateManager.ChangeState(Model_Level::stateManager.MAIN_MENU);
	}
}

void Model_Screen::UpdateMainMenu(double dt, bool* myKeys, double mouse_x, double mouse_y)
{
	/* Update cursor */
	cursor.Follow(mouse_x, mouse_y);	//hard coded console height

										/* Check collide */
	if (pressTimer > 1)
	{
		if (cursor.QuickAABBDetection(&start_Game) && myKeys[KEY_LMOUSE])	//pressed start game button
		{
			start_Game.SetActive(false);
			instruction.SetActive(false);
			go_back.SetActive(true);
			loseGame.SetActive(false);
			state = StateManager::GAME;
			//Model_Level::stateManager.ChangeState(Model_Level::stateManager.GAME);
			pressTimer = 0;
		}
		else if (cursor.QuickAABBDetection(&instruction) && myKeys[KEY_LMOUSE])	//pressed instructions
		{
			start_Game.SetActive(false);
			instruction.SetActive(false);
			go_back.SetActive(true);
			state = StateManager::INSTRUCTION;
			//Model_Level::stateManager.ChangeState(Model_Level::stateManager.INSTRUCTION);
			pressTimer = 0;
		}
	}
}

void Model_Screen::UpdateWinScreen(double dt, bool* myKeys, double mouse_x, double mouse_y)
{
	/* Update cursor */
	cursor.Follow(mouse_x, mouse_y);	//hard coded console height

	start_Game.SetActive(false);
	instruction.SetActive(false);
	go_back.SetActive(false);
	backmenu.SetActive(true);
	restart.SetActive(false);
	winGame.SetActive(true);

	if (pressTimer > 1)
	{
		if (cursor.QuickAABBDetection(&backmenu) && myKeys[KEY_LMOUSE])	//
		{
			backmenu.SetActive(false);
			winGame.SetActive(false);
			loseGame.SetActive(false);
			state = StateManager::MAIN_MENU;
			start_Game.SetActive(true);
			instruction.SetActive(true);
			pressTimer = 0;
		}
	}
}

void Model_Screen::UpdateLoseScreen(double dt, bool* myKeys, double mouse_x, double mouse_y)
{
	/* Update cursor */
	cursor.Follow(mouse_x, mouse_y);	//hard coded console height

	start_Game.SetActive(false);
	instruction.SetActive(false);
	go_back.SetActive(false);
	backmenu.SetActive(true);
	restart.SetActive(true);
	winGame.SetActive(false);
	loseGame.SetActive(true);
	if (pressTimer > 1)
	{
	if (cursor.QuickAABBDetection(&restart) && myKeys[KEY_LMOUSE])	//
	{
		restart.setActive(false);
		state = StateManager::GAME;
		pressTimer = 0;
	}

	else if (cursor.QuickAABBDetection(&backmenu) && myKeys[KEY_LMOUSE])	//
	{
		backmenu.SetActive(false);
		state = StateManager::MAIN_MENU;
		start_Game.SetActive(true);
		instruction.SetActive(true);
		loseGame.SetActive(false);
		pressTimer = 0;
	}
}
}

void Model_Screen::Exit()
{
	Model::Exit();
}