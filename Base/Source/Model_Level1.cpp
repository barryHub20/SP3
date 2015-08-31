#include "Model_Level1.h"
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
Model_Level1::Model_Level1()
{

}

Model_Level1::~Model_Level1()
{
}

/*********** core functions ***************/
void Model_Level1::Init()
{
	if(!initBasicsAlready)
	{
		initBasicsAlready = true;
		Model_Level::Init();

		/* timer and delay time */
		delayTime = 0.3;
		keyPressedTimer = delayTime;

	stopGame = false;
	doorUnlocked = false;
	haveFire = true;
	Timer = 0;
	mapTimer = 0;
	openTutorial = false;
	pauseGame = true;
	pressTimer = 0;

		//object
		InitObject();

		//player
		if(player != NULL)
		{
			goList.push_back(player);

			/* Init static sprite (Player) */
			InitStaticSprite();
		}

		spawnItems();
		InitTrigger();

		//Model_Level::mapManager.SetMap(0);	//set to map 0 first
		/* set current level map to level 1 */
		level_map = Model_Level::mapManager.GetMap(LEVEL_1);

		/* Set map scale: for camera */
		//go model_level::Init_map() to see how big ur world is,
		//based on floor layer (or any other layer that is the biggest)
		//width: num_ofTileWidth * tileSize
		//height: num_ofTileHeight * tileSize
		mapSize.Set(16 * 64, 13 * 64, 1);

		camera.Init(Vector3(0, -130, 0), Vector3(0, 0, -10), Vector3(0, 1, 0), m_view_width * 0.2f,  m_view_height * 0.2f
			, m_view_width, m_view_height, mapSize.x, mapSize.y);

		//UI
		InitUI();

		//sprite (Only level one call once)
		InitSprite();

		//Init puzzle
		puzzleManager = new PuzzleManager;
		puzzleManager->Init(MapManager::MAX_MAP);
		InitPuzzles();
	}

	/* set bounds so camera spawns at correct place each time reenter this level */
	camera.SetBound(player->getPosition());
}

void Model_Level1::InitObject()
{	
	/** Set up player **/
	ReadFromFile("Save_Load_File_lvl1.txt");
}

void Model_Level1::InitTrigger()
{
	triggerObject.resize(5);
	triggerObject[0] = new TriggerObject(Geometry::meshList[Geometry::GEO_NOTTRIGGER], TriggerObject::FIRETRIGGER, Vector3(750, 560, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	goList.push_back(triggerObject[0]);

	triggerObject[1] = new TriggerObject(Geometry::meshList[Geometry::GEO_ISTRIGGER], TriggerObject::FIRETRIGGER, Vector3(750, 560, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	goList.push_back(triggerObject[1]);

	//Fire
	triggerObject[2] = new TriggerObject(Geometry::meshList[Geometry::GEO_FIRE], TriggerObject::FIRE, Vector3(550, 688, 0), Vector3(30, 158, 1), 0, true, *sfx_man, player);
	triggerObject[2]->storeSpriteAnimation("fire trap", 1, 7, "Image//Sprites//fire.tga");
	triggerObject[2]->processSpriteAnimation(TriggerObject::FIRE, 0.8f, 1, 0, 6, 0, 1, false);
	goList.push_back(triggerObject[2]);

	triggerObject[3] = new TriggerObject(Geometry::meshList[Geometry::GEO_FIRE], TriggerObject::FIRE, Vector3(680, 688, 0), Vector3(30, 158, 1), 0, true, *sfx_man, player);
	triggerObject[3]->storeSpriteAnimation("fire trap", 1, 7, "Image//Sprites//fire.tga");
	triggerObject[3]->processSpriteAnimation(TriggerObject::FIRE, 1.f, 1, 0, 6, 0, 1, false);
	goList.push_back(triggerObject[3]);

	//Arrow trap
	triggerObject[4] = new TriggerObject(Geometry::meshList[Geometry::GEO_ARROWLEFT], TriggerObject::ARROWTRAP, Vector3(940, 100, 0), Vector3(30, 30, 1), false, true, *sfx_man, player);
	goList.push_back(triggerObject[4]);
}

void Model_Level1::InitUI()
{
	wordBox = new UI_Object;
	wordBox->Init(Geometry::meshList[Geometry::GEO_WORDBOX], Vector3(80, 60, 1), Vector3(85, 80, 1), "Tutorial_Box", UI_Object::TUTORIAL_BIG, true);
	UI_List.push_back(wordBox);
}

void Model_Level1::InitPuzzles()
{
	puzzleManager->addTextPuzzle(MapManager::MAP1, "test1");
	puzzleManager->addTextPuzzle(MapManager::MAP1, "test2");
	puzzleManager->addPicturePuzzle(MapManager::MAP1, "Image//Sprites//guard.tga");
	puzzleManager->addTextPuzzle(MapManager::MAP2, "test3");
	puzzleManager->addTextPuzzle(MapManager::MAP3, "test4");

	/*cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
	puzzleManager->goToNextPart();
	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
	puzzleManager->goToNextPart();
	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
	puzzleManager->goToNextPart();
	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
	puzzleManager->goToNextPart();
	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
	puzzleManager->goToNextPart();
	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;*/
}

void Model_Level1::spawnItems()
{
	item = new Item(Geometry::meshList[Geometry::GEO_KEYY], Item::KEY, true, Vector3(200, 500, 0), Vector3(35, 35, 1));
	goList.push_back(item);
	itemList.push_back(item);
}

void Model_Level1::Update(double dt, bool* myKeys, Vector3 mousePos)
{
	/* parent class update */
	Model_Level::Update(dt, myKeys, mousePos);

	if(keyPressedTimer < delayTime)
		keyPressedTimer += dt;
	/* Update based on states */
	switch (Model_Level::stateManager.GetState())
	{
	case StateManager::MAIN_MENU:
		UpdateMainMenu(dt, myKeys, mousePos.x,  mousePos.y);
		break;
	case StateManager::GAME:
		UpdateGame(dt, myKeys);
		break;
	case StateManager::INSTRUCTION:
		UpdateInstructions(dt, myKeys, mousePos.x, mousePos.y);
		break;
	}

	/* If in transition */
	if (Model_Level::stateManager.isTransition())
	{
		Model_Level::stateManager.UpdateTransitionTime(dt);
	}
}

void Model_Level1::UpdateGame(double dt, bool* myKeys)
{
	UpdateTutorial(dt, myKeys);
	if (pauseGame == false)
	{
		// Sound - ambience
		sfx_man->play_ambience();

		/* Update player */
		player->Update(dt, myKeys);

		//Update the traps
		UpdateTraps(dt, myKeys);

		if(myKeys[KEY_K])
		{
			player->Translate(Vector3(659, 389, 0));
		}

		player->dropItem(dt, item, myKeys);

		/* check collision with object */
		//start: Set up collision bound before checking with the others
		player->StartCollisionCheck();

		//getCamera()->position.Set(player->getPosition().x-500, player->getPosition().y-400, 1);
		//getCamera()->target.Set(player->getPosition().x-500, player->getPosition().y-400, 0);

		//cout << player->getPosition() << endl;

		for (int i = 0; i < level_map->size(); i++)
		{
			if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
			{
				//(*(*level_map))[i]->getWalkable(player->getPosition().x, player->getPosition().y);
			}
		}

		/* check collision with object */
		//start: Set up collision bound before checking with the others
		player->StartCollisionCheck();

		/* check collision with map */
		for (int i = 0; i < level_map->size(); i++)
		{
			if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
			{
				(*level_map)[i]->getWalkable(player->getPosition().x, player->getPosition().y);
			}
		}

		/* check collision with map */
		for (int i = 0; i < level_map->size(); i++)
		{
			if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
			{
				(*level_map)[i]->CheckCollisionWith(player);
			}
		}

		if (door->getActive())
		{
			if (player->CollisionCheck(door))
			{
				if (doorUnlocked)
				{
					door->setActive(false);
					sfx_man->play_unlock();
				}
			}
		}

		player->dropItem(dt, item, myKeys);

		/*** Change to next level ***/
		mapTimer += dt;
		if (player->CollisionCheck(staircase))
		{
			if (mapTimer > 5)
			{
				goNextLevel = true;
				//Model_Level::mapManager.ChangeNextMap();
				mapTimer = 0;
			}
		}

		//player->dropItem(dt, item, myKeys);

		player->getCollideBound()->Reset();

		/* Collision response */
		player->CollisionResponse();	//translate to new pos if collides

		/* Test pick up items */
		for (int i = 0; i < itemList.size(); ++i)
		{
			if (player->pickUp(itemList[i], myKeys))	//if successfully pick up
			{
				//if item is key
				//cout << itemList[i]->getItemID() << endl;
				if (itemList[i]->getItemID() == Item::KEY)
				{
					doorUnlocked = true;
				}
			}
		}

		player->useItem(myKeys);

		/* Update target */
		camera.target = camera.position;
		camera.target.z -= 10;

		/* Press space to go back main menu */
		if (myKeys[KEY_SPACE] && keyPressedTimer >= delayTime)
		{
			keyPressedTimer = 0.0;
			//stateManager->ChangeState(stateManager->MAIN_MENU);
		}

		/* Key Q to open puzzle */
		static bool ButtonQState = false;
		if (!ButtonQState && myKeys[KEY_Q])
		{
			ButtonQState = true;
			std::cout << "QBUTTON DOWN" << std::endl;
			puzzleOpen = true;
		}
		else if (ButtonQState && !(myKeys[KEY_Q]))
		{
			ButtonQState = false;
			std::cout << "QBUTTON UP" << std::endl;
			puzzleOpen = false;
		}

		/* Load/change map */
		//Key B to move to next map (RP)
		static bool ButtonBState = false;
		if (!ButtonBState && myKeys[KEY_B])
		{
			ButtonBState = true;
			std::cout << "BBUTTON DOWN" << std::endl;
			//stateManager->ChangeState(StateManager::MAIN_MENU);
			//mapManager->ChangeNextMap();
			puzzleManager->goToNextPart();
		}
		else if (ButtonBState && !(myKeys[KEY_B]))
		{
			ButtonBState = false;
			std::cout << "BBUTTON UP" << std::endl;
		}

		if (player->getHealth() == 0)
		{
			stopGame = true;
		}
	}
}

void Model_Level1::UpdateTutorial(double dt, bool* myKeys)
{
	openTutorial = true;
	if(myKeys[KEY_W] || myKeys[KEY_S] || myKeys[KEY_A] || myKeys[KEY_D])
	{
		openTutorial = false;
		pauseGame = false;
		//wasdFinish = true;
		wordBox->setActive(false);
	}
}

void Model_Level1::UpdateTraps(double dt, bool* myKeys)
{
	Timer += dt;
	/* check with trigger objects fire */
	for(int i = 0; i < 4; i++)
	{
		triggerObject[i]->Update(dt, myKeys); //animation and actual update
	}

	//Render
	if(triggerObject[0]->getTriggered() == false) //If lever is switched off
	{
		triggerObject[0]->setActive(false); //to change lever position
		triggerObject[1]->setActive(true); 
		triggerObject[2]->setActive(false); //fire
		triggerObject[3]->setActive(false); //fire
		haveFire = false; //do not render fire
	}
	else 
	{
		triggerObject[0]->setActive(true); 
		triggerObject[1]->setActive(false);
		triggerObject[2]->setActive(true); //fire on
		triggerObject[3]->setActive(true); //fire on
		haveFire = true;
	}

	//Timer for lever and damage, health
	if(Timer >= 0.5)
	{
		if(player->QuickAABBDetection(triggerObject[2]) && haveFire == true || player->QuickAABBDetection(triggerObject[3]) && haveFire == true)
		{
			player->setHealth(player->getHealth() - 15);
			player->Translate(player->getPosition() - 45);
			Timer = 0;
		}
		if (player->QuickAABBDetection(triggerObject[4]))
		{
			player->setHealth(player->getHealth() - 15);
			player->Translate(player->getPosition() - 45);
			Timer = 0;
		}
	}

}

void Model_Level1::UpdateInstructions(double dt, bool* myKeys, double mouse_x, double mouse_y)
{
	/* Update cursor */
	cursor.Follow(mouse_x, mouse_y);	//hard coded console height

	/* Check collide */
	if(cursor.QuickAABBDetection(&go_back) && myKeys[KEY_LMOUSE])	//go back to main menu
	{
		go_back.SetActive(false);
		start_Game.SetActive(true);
		instruction.SetActive(true);
		Model_Level::stateManager.ChangeState(Model_Level::stateManager.MAIN_MENU);
	}
}

void Model_Level1::UpdateMainMenu(double dt, bool* myKeys, double mouse_x, double mouse_y)
{
	/* Update cursor */
	cursor.Follow(mouse_x, mouse_y);	//hard coded console height

	/* Check collide */
	if(cursor.QuickAABBDetection(&start_Game) && myKeys[KEY_LMOUSE])	//pressed start game button
	{
		start_Game.SetActive(false);
		instruction.SetActive(false);
		go_back.SetActive(true);
		Model_Level::stateManager.ChangeState(Model_Level::stateManager.GAME);
	}
	else if(cursor.QuickAABBDetection(&instruction) && myKeys[KEY_LMOUSE])	//pressed instructions
	{
		start_Game.SetActive(false);
		instruction.SetActive(false);
		go_back.SetActive(true);
		Model_Level::stateManager.ChangeState(Model_Level::stateManager.INSTRUCTION);
	}
}

void Model_Level1::Exit()
{
	Model::Exit();
}