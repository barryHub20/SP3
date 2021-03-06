#include "Model_Level3.h"
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
Model_Level3::Model_Level3()
{
	
}

Model_Level3::~Model_Level3()
{
}

/*********** core functions ***************/
void Model_Level3::Init()
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
		doorRUnlocked = false;
		doorPUnlocked = false;
		doorGUnlocked = false;
		haveFire = true;
		Timer = 0;
		mapTimer = 0;

		state = StateManager::GAME;

		//player
		if(player != NULL)
		{
			goList.push_back(player);
		}

		/* !! Remember to set player pos to where ever you want */
		player->translate(80, 70, 1);

		//object
		InitObject();
		spawnItems();
		InitTrigger();

		//UI
		InitUI();

		//Model_Level::mapManager.SetMap(0);	//set to map 0 first
		/* set current level map to level 1 */
		level_map = Model_Level::mapManager.GetMap(LEVEL_3);

		/* Set map scale: for camera */
		//go model_level::Init_map() to see how big ur world is,
		//based on floor layer (or any other layer that is the biggest)
		//width: num_ofTileWidth * tileSize
		//height: num_ofTileHeight * tileSize
		mapSize.Set(16 * 64, 13 * 64, 1);

		camera.Init(Vector3(0, -130, 0), Vector3(0, 0, -10), Vector3(0, 1, 0), m_view_width * 0.2f,  m_view_height * 0.2f
			, m_view_width, m_view_height, mapSize.x, mapSize.y);

		//Init puzzle
		puzzleManager = new PuzzleManager;
		puzzleManager->Init(MapManager::MAX_MAP);
		InitPuzzles();

		/* Set original player position */
		originalPos.Set(100, 90, 2);

		/* Coin list */
		if(player != NULL)
		{
			for(int i = 0; i < player->coinList.size(); ++i)
			{
				goList.push_back(player->coinList[i]);
			}
		}
	}

	//set UI
	main_UI_bar.SetActive(true);

	/* set player */
	player->Translate(originalPos);

	/* set bounds so camera spawns at correct place each time reenter this level */
	camera.SetBound(player->getPosition());

	/* Clear inventory */
	player->getInventory()->clearFromInventory(Item::KEY);
	player->getInventory()->clearFromInventory(Item::NOTE);

	/** init **/
	for(std::vector<GameObject*>::iterator it = goList.begin(); it != goList.end(); ++it)
	{
		Object *go = (Object *)*it;
		go->Init();
	}
}

void Model_Level3::InitUI()
{
	Vector3 winDimension(m_2D_view_width/2, m_2D_view_height/2, 1);
}


void Model_Level3::InitObject()
{	
	/** Set up player **/
	ReadFromFile("Save_Load_File_lvl3.txt");

	/** Set up enemy **/
	ogres[0] = new Ogre(Geometry::meshList[Geometry::GEO_CUBE], Vector3(900, 700, 0), Vector3(50, 50, 1), 0, 10, true);
	goList.push_back(ogres[0]);

	// Post for bottom middle key
	ogres[1] = new Ogre(Geometry::meshList[Geometry::GEO_CUBE], Vector3(540, 210, 0), Vector3(50, 50, 1), 0, 10, true);
	goList.push_back(ogres[1]);

	// Post for bottom left key
	ogres[2] = new Ogre(Geometry::meshList[Geometry::GEO_CUBE], Vector3(230, 400, 0), Vector3(50, 50, 1), 0, 10, true);
	goList.push_back(ogres[2]);
}

void Model_Level3::InitTrigger()
{
	triggerObject.resize(3);

	triggerObject[0] = new TriggerObject(Geometry::meshList[Geometry::GEO_NOTTRIGGER], TriggerObject::FIRETRIGGER, Vector3(-10, -10, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	goList.push_back(triggerObject[0]);

	triggerObject[1] = new TriggerObject(Geometry::meshList[Geometry::GEO_ISTRIGGER], TriggerObject::FIRETRIGGER, Vector3(-10, -10, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	goList.push_back(triggerObject[1]);

	//Arrow trap
	triggerObject[2] = new TriggerObject(Geometry::meshList[Geometry::GEO_ARROWLEFT], TriggerObject::ARROWTRAP, Vector3(0, 0, 0), Vector3(30, 30, 1), false, true, *sfx_man, player);
	goList.push_back(triggerObject[2]);
}

void Model_Level3::InitPuzzles()
{
	/*puzzleManager->addTextPuzzle(MapManager::MAP1, "test1");
	puzzleManager->addTextPuzzle(MapManager::MAP1, "test2");
	puzzleManager->addPicturePuzzle(MapManager::MAP1, "Image//Sprites//guard.tga");
	puzzleManager->addTextPuzzle(MapManager::MAP2, "test3");
	puzzleManager->addTextPuzzle(MapManager::MAP3, "test4");*/

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

void Model_Level3::spawnItems()
{
	// Top left key
	item = new Item(Geometry::meshList[Geometry::GEO_KEYP], Item::KEYP, true, Vector3(120, 680, 0), Vector3(35, 35, 1));
	goList.push_back(item);
	itemList.push_back(item);

	// Bottom left key
	item = new Item(Geometry::meshList[Geometry::GEO_KEYR], Item::KEYR, true, Vector3(250, 230, 0), Vector3(35, 35, 1));
	goList.push_back(item);
	itemList.push_back(item);

	// Bottom middle key
	item = new Item(Geometry::meshList[Geometry::GEO_KEYY], Item::KEY, true, Vector3(590, 210, 0), Vector3(35, 35, 1));
	goList.push_back(item);
	itemList.push_back(item);
}

void Model_Level3::Update(double dt, bool* myKeys, Vector3 mousePos, StateManager::STATES currentState)
{
	/* parent class update */
	Model_Level::Update(dt, myKeys, mousePos, currentState);

	if(keyPressedTimer < delayTime)
		keyPressedTimer += dt;
	
	/* Update game */
	UpdateGame(dt, myKeys);

	///* If in transition */
	//if (Model_Level::stateManager.isTransition())
	//{
	//	Model_Level::stateManager.UpdateTransitionTime(dt);
	//}
}

void Model_Level3::UpdateGame(double dt, bool* myKeys)
{
	if (stopGame == false)
	{
		// Sound - ambience
		sfx_man->play_ambience();

		//Update enemy
		UpdateEnemy(dt);

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

		if (door_R->getActive())
		{
			if (player->CollisionCheck(door_R))
			{
				if (doorRUnlocked)
				{
					door_R->setActive(false);
					sfx_man->play_unlock();
				}
			}
		}

		if (door_P->getActive())
		{
			if (player->CollisionCheck(door_P))
			{
				if (doorPUnlocked)
				{
					door_P->setActive(false);
					sfx_man->play_unlock();
				}
			}
		}

/*		// Null
		if (door_G->getActive())
		{
			if (player->CollisionCheck(door_G))
			{
				if (doorGUnlocked)
				{
					door_G->setActive(false);
					sfx_man->play_unlock();
				}
			}
		} */

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
				if (itemList[i]->getItemID() == Item::KEYR)
				{
					doorRUnlocked = true;
				}
				if (itemList[i]->getItemID() == Item::KEYP)
				{
					doorPUnlocked = true;
				}
				if (itemList[i]->getItemID() == Item::KEYG)
				{
					doorGUnlocked = true;
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

		/* GO TO NEXT LEVEL (PRESS B FOR NOW) */
		//Key B to move to next map (RP)
		static bool ButtonBState = false;
		if (!ButtonBState && myKeys[KEY_B])
		{
			ButtonBState = true;
			goNextLevel = true;	//go to next level
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

void Model_Level3::UpdateTraps(double dt, bool* myKeys)
{
	Timer += dt;
	/* check with trigger objects fire */
	for(int i = 0; i < 3; i++)
	{
		triggerObject[i]->Update(dt, myKeys); //animation and actual update
	}

	//Render
	if(triggerObject[0]->getTriggered() == false) //If lever is switched off
	{
		triggerObject[0]->setActive(false); //to change lever position
		triggerObject[1]->setActive(true); 
		triggerObject[2]->setActive(false); //fire
		haveFire = false; //do not render fire
	}
	else 
	{
		triggerObject[0]->setActive(true); 
		triggerObject[1]->setActive(false);
		triggerObject[2]->setActive(true); //fire on
		haveFire = true;
	}
}

void Model_Level3::UpdateEnemy(double dt)
{
	for(int i = 0; i < 3; i++)
	{
		ogres[i]->Update(dt, level_map, goList);

		/* start set up */
		ogres[i]->StartCollisionCheck();

		/* check with wall */
		for (int i = 0; i < (*level_map).size(); i++)
		{
			(*level_map)[i]->CheckCollisionWith(ogres[i]);
		}

		/* check with all other objects */
		ogres[i]->getCollideBound()->Reset();

		//response
		ogres[i]->CollisionResponse();
	}
}

void Model_Level3::ClearLevel()
{
	Model_Level::ClearLevel();
}

void Model_Level3::Exit()
{
	Model::Exit();
}