#include "Model_Level4.h"
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
Model_Level4::Model_Level4()
{

}

Model_Level4::~Model_Level4()
{
}

/*********** core functions ***************/
void Model_Level4::Init()
{
	if (!initBasicsAlready)
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

		invulerabilityFrame = 1.0f;
		damageTimer = 0.f;

		//player
		if (player != NULL)
		{
			goList.push_back(player);
		}

		/* !! Remember to set player pos to where ever you want */
		player->translate(100, 100, 1);

		//object
		InitObject();
		spawnItems();
		InitTrigger();

		//UI
		InitUI();

		//Init puzzle
		puzzleManager = new PuzzleManager;
		puzzleManager->Init(MapManager::MAX_MAP);
		InitPuzzles();

		/* set current level map to level 4 */
		level_map = Model_Level::mapManager.GetMap(LEVEL_4);

		/* Set map scale: for camera */
		//go model_level::Init_map() to see how big ur world is,
		//based on floor layer (or any other layer that is the biggest)
		//width: num_ofTileWidth * tileSize
		//height: num_ofTileHeight * tileSize
		mapSize.Set(48 * 32, 37 * 32, 1);

		camera.Init(Vector3(0, -120, 0), Vector3(0, 0, -10), Vector3(0, 1, 0), m_view_width * 0.2f, m_view_height * 0.2f
			, m_view_width, m_view_height, mapSize.x, mapSize.y);
	}
	
	camera.SetBound(player->getPosition());
}

void Model_Level4::InitUI()
{
	Vector3 winDimension(m_2D_view_width / 2, m_2D_view_height / 2, 1);
}

void Model_Level4::InitObject()
{
	/** Set up player **/
	ReadFromFile("Save_Load_File_lvl4.txt");

	/** Set up enemy **/
	E_Ogre = new Ogre(Geometry::meshList[Geometry::GEO_CUBE], Vector3(700, 600, 0), Vector3(50, 50, 1), 0, 10, true);
	goList.push_back(E_Ogre);

	/** init **/
	for (std::vector<GameObject*>::iterator it = goList.begin(); it != goList.end(); ++it)
	{
		Object *go = (Object *)*it;
		go->Init();
	}

	//door 1
	doors[0] = new GameObject(Geometry::meshList[Geometry::GEO_DOORY], Vector3(720, 80, 0), Vector3(40, 95, 1), true);
	goList.push_back(doors[0]);

	//door 2
	doors[1] = new GameObject(Geometry::meshList[Geometry::GEO_DOORY], Vector3(1360, 680, 0), Vector3(40, 95, 1), true);
	goList.push_back(doors[1]);
}

int Model_Level4::triggerObjectSize = 12;

void Model_Level4::InitTrigger()
{
	triggerObject.resize(triggerObjectSize);

	//Fire lever
	triggerObject[0] = new TriggerObject(Geometry::meshList[Geometry::GEO_NOTTRIGGER], TriggerObject::FIRETRIGGER, Vector3(100, 100, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	goList.push_back(triggerObject[0]);

	triggerObject[1] = new TriggerObject(Geometry::meshList[Geometry::GEO_ISTRIGGER], TriggerObject::FIRETRIGGER, Vector3(100, 100, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	goList.push_back(triggerObject[1]);

	//Fire
	triggerObject[2] = new TriggerObject(Geometry::meshList[Geometry::GEO_FIRE], TriggerObject::FIRE, Vector3(490, 110, 0), Vector3(30, 158, 1), 0, true, *sfx_man, player);
	triggerObject[2]->storeSpriteAnimation("fire trap", 1, 7, "Image//Sprites//fire.tga");
	triggerObject[2]->processSpriteAnimation(TriggerObject::FIRE, 0.8f, 1, 0, 6, 0, 1, false);
	goList.push_back(triggerObject[2]);

	//Fire lever 2
	triggerObject[3] = new TriggerObject(Geometry::meshList[Geometry::GEO_NOTTRIGGER], TriggerObject::FIRETRIGGER, Vector3(600, 200, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	goList.push_back(triggerObject[3]);

	triggerObject[4] = new TriggerObject(Geometry::meshList[Geometry::GEO_ISTRIGGER], TriggerObject::FIRETRIGGER, Vector3(600, 200, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	goList.push_back(triggerObject[4]);

	//Fire 2
	triggerObject[5] = new TriggerObject(Geometry::meshList[Geometry::GEO_FIRE], TriggerObject::FIRE, Vector3(685, 660, 0), Vector3(30, 158, 1), 0, true, *sfx_man, player);
	triggerObject[5]->storeSpriteAnimation("fire trap", 1, 7, "Image//Sprites//fire.tga");
	triggerObject[5]->processSpriteAnimation(TriggerObject::FIRE, 0.8f, 1, 0, 6, 0, 1, false);
	goList.push_back(triggerObject[5]);

	//Arrow pressure plate
	triggerObject[6] = new TriggerObject(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], TriggerObject::ARROWTRIGGER, Vector3(270, 850, 0), Vector3(480, 50, 1), 0, true, *sfx_man, player);
	triggerObject[6]->setTriggered(false);
	goList.push_back(triggerObject[6]);

	triggerObject[7] = new TriggerObject(Geometry::meshList[Geometry::GEO_ARROWRIGHT], TriggerObject::ARROWTRAP, Vector3(65, 700, 0), Vector3(30, 30, 1), 0, false, *sfx_man, player);
	triggerObject[7]->setTriggered(false);
	goList.push_back(triggerObject[7]);

	//Arrow pressure plate 2
	triggerObject[8] = new TriggerObject(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], TriggerObject::ARROWTRIGGER, Vector3(270, 650, 0), Vector3(480, 50, 1), 0, true, *sfx_man, player);
	triggerObject[8]->setTriggered(false);
	goList.push_back(triggerObject[8]);

	triggerObject[9] = new TriggerObject(Geometry::meshList[Geometry::GEO_ARROWRIGHT], TriggerObject::ARROWTRAP, Vector3(45, 500, 0), Vector3(30, 30, 1), 0, false, *sfx_man, player);
	triggerObject[9]->setTriggered(false);
	goList.push_back(triggerObject[9]);

	//Arrow pressure plate 3
	triggerObject[10] = new TriggerObject(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], TriggerObject::ARROWTRIGGER, Vector3(1380, 480, 0), Vector3(50, 170, 1), 0, true, *sfx_man, player);
	triggerObject[10]->setTriggered(false);
	goList.push_back(triggerObject[10]);

	triggerObject[11] = new TriggerObject(Geometry::meshList[Geometry::GEO_ARROWRIGHT], TriggerObject::ARROWTRAP, Vector3(560, 480, 0), Vector3(30, 30, 1), 0, false, *sfx_man, player);
	triggerObject[11]->setTriggered(false);
	triggerObject[11]->speed = 15.f;
	goList.push_back(triggerObject[11]);

	//Spike traps
	spikeTraps[0] = new TriggerObject(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], TriggerObject::SPIKEREAPPEAR, Vector3(800, 480, 1), Vector3(50, 180, 1), 0.f, true, *sfx_man, player);
	spikeTraps[0]->duration = 1.f;
		//GameObject(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], Vector3(800, 480, 1), Vector3(50, 180, 1), true);
	goList.push_back(spikeTraps[0]);

	spikeTraps[1] = new TriggerObject(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], TriggerObject::SPIKEREAPPEAR, Vector3(1000, 480, 1), Vector3(50, 180, 1), 0.f, true, *sfx_man, player );
	spikeTraps[1]->duration = 1.5f;
		//GameObject(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], Vector3(1000, 480, 1), Vector3(50, 180, 1), true);
	goList.push_back(spikeTraps[1]);

	spikeTraps[2] = new TriggerObject(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], TriggerObject::SPIKEREAPPEAR, Vector3(1200, 480, 1), Vector3(50, 180, 1), 0.f, true, *sfx_man, player);
	spikeTraps[2]->duration = 1.f;
		//GameObject(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], Vector3(1200, 480, 1), Vector3(50, 180, 1), true);
	goList.push_back(spikeTraps[2]);

	//Arrow trap
	//triggerObject[2] = new TriggerObject(Geometry::meshList[Geometry::GEO_ARROWLEFT], TriggerObject::ARROWTRAP, Vector3(940, 100, 0), Vector3(30, 30, 1), false, true, *sfx_man, player);
	//goList.push_back(triggerObject[2]);
}

void Model_Level4::InitPuzzles()
{
	puzzleManager->addTextPuzzle(MapManager::MAP1, "blue crate");
	puzzleManager->addTextPuzzle(MapManager::MAP1, "closed pot");
	puzzleManager->addTextPuzzle(MapManager::MAP1, "next floor");
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

void Model_Level4::spawnItems()
{
	keys[0] = new Item(Geometry::meshList[Geometry::GEO_KEYY], Item::KEY, true, Vector3(60, 180, 0), Vector3(25, 25, 1));
	goList.push_back(keys[0]);
	itemList.push_back(keys[0]);

	keys[1] = new Item(Geometry::meshList[Geometry::GEO_KEYY], Item::KEY, true, Vector3(1300, 315, 0), Vector3(25, 25, 1));
	goList.push_back(keys[1]);
	itemList.push_back(keys[1]);
}

void Model_Level4::Update(double dt, bool* myKeys, Vector3 mousePos)
{
	/* parent class update */
	Model_Level::Update(dt, myKeys, mousePos);

	if (keyPressedTimer < delayTime)
		keyPressedTimer += dt;

	/* Update game */
	UpdateGame(dt, myKeys);

	/* If in transition */
	if (Model_Level::stateManager.isTransition())
	{
		Model_Level::stateManager.UpdateTransitionTime(dt);
	}
}

void Model_Level4::UpdateGame(double dt, bool* myKeys)
{
	//cout << triggerObject[7]->getPosition() << endl;
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

		//Drop item
		player->dropItem(dt, item, myKeys);

		/* check collision with object */
		//start: Set up collision bound before checking with the others
		player->StartCollisionCheck();

		getCamera()->position.Set(player->getPosition().x-500, player->getPosition().y-400, 1);
		getCamera()->target.Set(player->getPosition().x-500, player->getPosition().y-400, 0);

		/* check collision with object */
		//start: Set up collision bound before checking with the others
		player->StartCollisionCheck();

		/* check collision with map */
		for (int i = 0; i < level_map->size(); i++)
		{
			if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
			{
				if ((*level_map)[i]->CheckCollisionWith(player)) //check collision with player
				{
					
				}
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if (doors[i]->getActive())
			{
				if (player->CollisionCheck(doors[i]))
				{
					if (pickedUpKeys[i] == true)
					{
						doors[i]->setActive(false);
						sfx_man->play_unlock();
					}
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
				if (itemList[i]->getItemID() == Item::KEY)
				{
					pickedUpKeys[i] = true;
					puzzleManager->goToNextPart();
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

void Model_Level4::UpdateTraps(double dt, bool* myKeys)
{
	damageTimer += dt;
	/* check with trigger objects fire */
	for (int i = 0; i < triggerObjectSize; i++)
	{
		triggerObject[i]->Update(dt, myKeys); //animation and actual update
	}

	for (int i = 0; i < 3; i++)
	{
		spikeTraps[i]->Update(dt, myKeys);
	}

	//Render
	//Fire lever
	if (triggerObject[0]->getTriggered() == false) //If lever is switched off
	{
		triggerObject[0]->setActive(false); //to change lever position
		triggerObject[1]->setActive(true);
		triggerObject[2]->setActive(false); //fire
	}
	else
	{
		triggerObject[0]->setActive(true);
		triggerObject[1]->setActive(false);
		triggerObject[2]->setActive(true); //fire on
	}

	if (triggerObject[3]->getTriggered() == false) //If lever is switched off
	{
		triggerObject[3]->setActive(false); //to change lever position
		triggerObject[4]->setActive(true);
		triggerObject[5]->setActive(false); //fire
	}
	else
	{
		triggerObject[3]->setActive(true);
		triggerObject[4]->setActive(false);
		triggerObject[5]->setActive(true); //fire on
	}

	//Fire response
	if (damageTimer > invulerabilityFrame)
	{
		if (triggerObject[2]->QuickAABBDetection(player) || triggerObject[5]->QuickAABBDetection(player))
		{
			player->setHealth(player->getHealth() - 20);
			player->translateObject(Vector3(-50, 0, 0));
			damageTimer = 0.f;
		}
	}

	//First arrow trap, activate/reactivate arrow trap
	if (triggerObject[7]->getTriggered() == false && triggerObject[7]->getActive() == false && triggerObject[7]->arrowCooldown > 1.f)
	{
		if (triggerObject[6]->getTriggered() == true)
		{
			triggerObject[7]->setTriggered(true);
			triggerObject[7]->setActive(true);
		}
	}
	//Arrow response
	if (triggerObject[7]->QuickAABBDetection(player) && triggerObject[7]->getTriggered() == true)
	{
		triggerObject[7]->resetPosition();
		triggerObject[7]->setTriggered(false);
		triggerObject[7]->setActive(false);
		triggerObject[7]->arrowCooldown = 0.f;
		player->setHealth(player->getHealth() - 20);
		player->translateObject(Vector3(10, 0, 0));
	}
	else if (triggerObject[7]->getPosition().x > 500)
	{
		triggerObject[7]->resetPosition();
		triggerObject[7]->setTriggered(false);
		triggerObject[7]->setActive(false);
		triggerObject[7]->arrowCooldown = 0.f;
	}

	//Second arrow trap
	if (triggerObject[9]->getTriggered() == false && triggerObject[9]->getActive() == false && triggerObject[9]->arrowCooldown > 1.f)
	{
		if (triggerObject[8]->getTriggered() == true)
		{
			triggerObject[9]->setTriggered(true);
			triggerObject[9]->setActive(true);
		}
	}

	if (triggerObject[9]->QuickAABBDetection(player) && triggerObject[9]->getTriggered() == true)
	{
		triggerObject[9]->resetPosition();
		triggerObject[9]->setTriggered(false);
		triggerObject[9]->setActive(false);
		triggerObject[9]->arrowCooldown = 0.f;
		player->setHealth(player->getHealth() - 20);
		player->translateObject(Vector3(10, 0, 0));
	}
	else if (triggerObject[9]->getPosition().x > 500)
	{
		triggerObject[9]->resetPosition();
		triggerObject[9]->setTriggered(false);
		triggerObject[9]->setActive(false);
		triggerObject[9]->arrowCooldown = 0.f;
	}

	//Third arrow trap
	if (triggerObject[11]->getTriggered() == false && triggerObject[11]->getActive() == false && triggerObject[11]->arrowCooldown > 1.f)
	{
		if (triggerObject[10]->getTriggered() == true)
		{
			triggerObject[11]->setTriggered(true);
			triggerObject[11]->setActive(true);
		}
	}

	if (triggerObject[11]->QuickAABBDetection(player) && triggerObject[11]->getTriggered() == true)
	{
		triggerObject[11]->resetPosition();
		triggerObject[11]->setTriggered(false);
		triggerObject[11]->setActive(false);
		triggerObject[11]->arrowCooldown = 0.f;
		player->setHealth(player->getHealth() - 20);
		player->translateObject(Vector3(10, 0, 0));
	}
	else if (triggerObject[11]->getPosition().x > 1470)
	{
		triggerObject[11]->resetPosition();
		triggerObject[11]->setTriggered(false);
		triggerObject[11]->setActive(false);
		triggerObject[11]->arrowCooldown = 0.f;
	}

	//Spikes
	if (damageTimer > invulerabilityFrame)
	{
		for (int i = 0; i < 3; i++)
		{
			if (spikeTraps[i]->QuickAABBDetection(player) && spikeTraps[i]->getActive() == true)
			{
				player->setHealth(player->getHealth() - 90);
				player->translateObject(Vector3(50, 0, 0));
				damageTimer = 0.f;
			}
		}
	}
	
	//for (int i = 0; i < level_map->size(); i++)
	//{
	//	if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
	//	{
	//		(*level_map)[i]->CheckCollisionWith(triggerObject[7]);
	//	}
	//}

	//cout << triggerObject[7]->getTriggered() << endl;
}

void Model_Level4::UpdateEnemy(double dt)
{
	E_Ogre->Update(dt, level_map, goList);
	//E_Ogre->Update(dt, &Model_Level::mapManager, goList);

	/* start set up */
	E_Ogre->StartCollisionCheck();

	/* check with wall */
	for (int i = 0; i < (*level_map).size(); i++)
	{
		(*level_map)[i]->CheckCollisionWith(E_Ogre);
	}

	/* check with all other objects */
	E_Ogre->getCollideBound()->Reset();

	//response
	E_Ogre->CollisionResponse();
}

void Model_Level4::Exit()
{
	Model::Exit();
}