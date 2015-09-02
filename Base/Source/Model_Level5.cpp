#include "Model_Level5.h"
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
Model_Level5::Model_Level5()
{
	
}

Model_Level5::~Model_Level5()
{
}

/*********** core functions ***************/
void Model_Level5::Init()
{
	if(!initBasicsAlready)
	{
		Model_Level::Init();

		/* timer and delay time for puzzle */
		delayTime = 0.3;
		keyPressedTimer = delayTime;

		/* State */
		state = StateManager::GAME;

		//player
		if(player != NULL)
		{
			player->translate(960,64, 1);
			goList.push_back(player);
		}

		/* !! Remember to set player pos to where ever you want */
		

		//object
		InitObject();
		spawnItems();
		InitTrigger();

		//Model_Level::mapManager.SetMap(0);	//set to map 0 first
		/* set current level map to level 1 */
		level_map = Model_Level::mapManager.GetMap(LEVEL_5);

		/* Set map scale: for camera */
		mapSize.Set(16 * 64, 13 * 64, 1);

		camera.Init(Vector3(0, -120, 0), Vector3(0, 0, -10), Vector3(0, 1, 0), m_view_width * 0.2f,  m_view_height * 0.2f
			, m_view_width, m_view_height, mapSize.x, mapSize.y);

		//Init puzzle
		puzzleManager = new PuzzleManager;
		puzzleManager->Init(MapManager::MAX_MAP);
		InitPuzzles();

		State = IM_1;

		initBasicsAlready = true;

		doorUnlocked = false;

		/* Set original player position */
		originalPos.Set(100, 100, 2);

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

	/* set player */
	player->Translate(originalPos);

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

void Model_Level5::InitObject()
{	
	/** Set up player **/
	ReadFromFile("Save_Load_File_lvl5.txt");

	/** Set up enemy **/
	E_Ogre = new Ogre(Geometry::meshList[Geometry::GEO_CUBE], Vector3(700, 600, 0), Vector3(50, 50, 1), 0, 10, true);
	goList.push_back(E_Ogre);

	/** init **/
	for(std::vector<GameObject*>::iterator it = goList.begin(); it != goList.end(); ++it)
	{
		Object *go = (Object *)*it;
		go->Init();
	}
}

void Model_Level5::InitTrigger()
{
	bool EndInit = false;
	
	for(int i = 0; EndInit == false; ++i)
	{
		triggerObject.resize(triggerObject.size()+1);
		switch(i)
		{
		case 0:
			{
				//Lever OFF For Fire1
				triggerObject[i] = new TriggerObject(Geometry::meshList[Geometry::GEO_NOTTRIGGER], TriggerObject::FIRETRIGGER, Vector3(60, 285, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
				goList.push_back(triggerObject[i]);
				break;
			}
		case 1:
			{
				//Lever ON For Fire1
				triggerObject[i] = new TriggerObject(Geometry::meshList[Geometry::GEO_ISTRIGGER], TriggerObject::FIRETRIGGER, Vector3(60, 285, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
				goList.push_back(triggerObject[i]);
				break;
			}
		case 2:
			{
				//Fire1
				triggerObject[i] = new TriggerObject(Geometry::meshList[Geometry::GEO_FIRE], TriggerObject::FIRE, Vector3(550, 720, -3), Vector3(30, 90, 1), 0, true, *sfx_man, player);
				triggerObject[i]->storeSpriteAnimation("fire trap", 1, 7, "Image//Sprites//fire.tga");
				triggerObject[i]->processSpriteAnimation(TriggerObject::FIRE, 0.8f, 1, 0, 6, 0, 1, false);
				goList.push_back(triggerObject[i]);
				break;
			}
		case 3:
			{
				//Lever OFF For Fire2
				triggerObject[i] = new TriggerObject(Geometry::meshList[Geometry::GEO_NOTTRIGGER], TriggerObject::FIRETRIGGER, Vector3(930,370, 1), Vector3(45, 45, 1), 0, true, *sfx_man, player);
				goList.push_back(triggerObject[i]);
				break;
			}
		case 4:
			{
				//Lever ON For Fire2
				triggerObject[i] = new TriggerObject(Geometry::meshList[Geometry::GEO_ISTRIGGER], TriggerObject::FIRETRIGGER, Vector3(930,370, 1), Vector3(45, 45, 1), 0, true, *sfx_man, player);
				goList.push_back(triggerObject[i]);
				break;
			}
		case 5:
			{
				//Fire2
				triggerObject[i] = new TriggerObject(Geometry::meshList[Geometry::GEO_FIRE], TriggerObject::FIRE, Vector3(650, 720, -3), Vector3(30, 90, 1), 0, true, *sfx_man, player);
				triggerObject[i]->storeSpriteAnimation("fire trap", 1, 7, "Image//Sprites//fire.tga");
				triggerObject[i]->processSpriteAnimation(TriggerObject::FIRE, 0.8f, 1, 0, 6, 0, 1, false);
				goList.push_back(triggerObject[i]);
				break;
			}
		case 7:
			{
				triggerObject[i] = new TriggerObject(Geometry::meshList[Geometry::GEO_ARROWLEFT], TriggerObject::ARROWTRAP, Vector3(0, 0, 0), Vector3(30, 30, 1), false, true, *sfx_man, player);
				goList.push_back(triggerObject[i]);
				break;
			}
		default:
			{
				EndInit = true;
				break;
			}
		}
	}

	 
	/* Trigger area */
	TriggerArea* ArrowFly = new TriggerArea;
	ArrowFly->Set(Vector3(827, 90, 1), Vector3(20, 20, 1), 5);	//press 5 times
	TriggerAreaList.push_back(ArrowFly);

	

	GameObject* obj = new GameObject;
	obj->Set("Debug cube for trigger area", Geometry::meshList[Geometry::GEO_DEBUG_CUBE], NULL, false, false);
	obj->translateObject(ArrowFly->position);
	obj->scaleObject(ArrowFly->scale.x, ArrowFly->scale.y, 1);
	goList.push_back(obj);

	/* unlock door */
	if(player->QuickAABBDetection(door_G))
	{
		if(!keyList[2]->getItemFloor())
		{
			door_G->setActive(false);
		}
	}

	if(player->QuickAABBDetection(door))
	{
		if(!keyList[0]->getItemFloor())
		{
			door->setActive(false);
		}
	}

	if(player->QuickAABBDetection(door_R))
	{
		if(!keyList[1]->getItemFloor())
		{
			door_R->setActive(false);
		}
	}
}

void Model_Level5::InitPuzzles()
{

}

void Model_Level5::spawnItems()
{
	keyList[0] = new Item(Geometry::meshList[Geometry::GEO_KEYY], Item::KEY, true, Vector3(800,85, 1), Vector3(35, 35, 1));
	goList.push_back(keyList[0]);
	itemList.push_back(keyList[0]);

	keyList[1] = new Item(Geometry::meshList[Geometry::GEO_KEYR], Item::KEYR, true, Vector3(500,85, 1), Vector3(35, 35, 1));
	goList.push_back(keyList[1]);
	itemList.push_back(keyList[1]);

	keyList[2] = new Item(Geometry::meshList[Geometry::GEO_KEYG], Item::KEYG, true, Vector3(60,600, 1), Vector3(35, 35, 1));
	goList.push_back(keyList[2]);
	itemList.push_back(keyList[2]);
}

void Model_Level5::Update(double dt, bool* myKeys, Vector3 mousePos, StateManager::STATES currentState)
{
	/* parent class update */
	Model_Level::Update(dt, myKeys, mousePos, currentState);
	//cout << player->getPosition() << endl;

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

void Model_Level5::UpdateGame(double dt, bool* myKeys)
{
	if (stopGame == false)
	{
		// Sound - ambience
		//sfx_man->play_ambience();

		//Update enemy
		UpdateEnemy(dt);
		
		//Update the traps
		UpdateTraps(dt, myKeys);

		/* Update player */
		player->Update(dt, myKeys);

		player->dropItem(dt, item, myKeys);
	
		/* check collision with object */
		//start: Set up collision bound before checking with the others
		player->StartCollisionCheck();


		/* check collision with map */
		for (int i = 0; i < level_map->size(); i++)
		{
			if ((*level_map)[i]->getMapType() == Map::COLLISIONMAP)
			{
				(*level_map)[i]->CheckCollisionWith(player);
			}
		}

		/* Update puzzle (put above check collision with door since we need response for door) */
		UpdatePuzzle(dt, myKeys);
		
		player->dropItem(dt, item, myKeys);

		

		/*** Change to next level ***/
		mapTimer += dt;
		if (player->CollisionCheck(staircase))
		{
			if (mapTimer > 5)
			{
				goNextLevel = true;
				mapTimer = 0;
			}
		}

		//Door & Key
		if(door->getActive() == true)
		{
			if(player->CollisionCheck(door))
			{
				if(doorUnlocked == true)
				{
					door->setActive(false);
				}
			}
		}
		if(door_R->getActive() == true)
		{
			if(player->CollisionCheck(door_R))
			{
				if(doorRUnlocked == true)
				{
					door_R->setActive(false);
				}
			}
		}
		if(door_G->getActive() == true)
		{
			if(player->CollisionCheck(door_G))
			{
				if(doorGUnlocked == true)
				{
					door_G->setActive(false);
				}
			}
		}

		/** Reset collision **/
		player->getCollideBound()->Reset();

		/** Collision response (if any) **/
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

		/** If stop game **/
		if (player->getHealth() == 0)
		{
			stopGame = true;
		}
	}

}

void Model_Level5::UpdateTraps(double dt, bool* myKeys)
{
	Timer += dt;
	/* check with trigger objects fire */
	for(int i = 0; i < triggerObject.size()-1; i++)
	{
		triggerObject[i]->Update(dt, myKeys); //animation and actual update
	}

	//Render
	for(int i = 0; i < triggerObject.size()-1; ++ i)
	{
		switch(i)
		{
		case 0:
			{
				if(triggerObject[i]->getTriggered() == false) //If lever is switched off
				{
					triggerObject[i]->setActive(false); //to change lever position
					triggerObject[i+1]->setActive(true); 
					triggerObject[i+2]->setActive(false); //fire
					haveFire = false; //do not render fire
				}
				else 
				{
					triggerObject[i]->setActive(true); 
					triggerObject[i+1]->setActive(false);
					triggerObject[i+2]->setActive(true); //fire on
					haveFire = true;
				}
			}
		case 3:
			{
				if(triggerObject[i]->getTriggered() == false) //If lever is switched off
				{
					triggerObject[i]->setActive(false); //to change lever position
					triggerObject[i+1]->setActive(true); 
					triggerObject[i+2]->setActive(false); //fire
					haveFire = false; //do not render fire
				}
				else 
				{
					triggerObject[i]->setActive(true); 
					triggerObject[i+1]->setActive(false);
					triggerObject[i+2]->setActive(true); //fire on
					haveFire = true;
				}
			}
		}
	}
	//Fire 1

	//Fire 2
	
	//Timer for lever and damage, health
	if(Timer >= 0.5)
	{
		if(player->QuickAABBDetection(triggerObject[2]) && haveFire == true || player->QuickAABBDetection(triggerObject[5]) && haveFire == true)
		{
			player->setHealth(player->getHealth() - 15);
			player->Translate(player->getPosition() + 45);
			Timer = 0;
		}
	}

	/* Update trigger area */
	for(int i = 0; i < TriggerAreaList.size(); ++i)
	{
		switch(i)
		{
		case 0:
			{
				if(keyPressedTimer >= delayTime)
				{
					if(myKeys[KEY_E])
					{
						keyPressedTimer = 0.0;
						TriggerAreaList[i]->QuickAABBDetection(player->getCollideBound(), true);
					}
				}
				/* if activated: get key and puzzle */
				if(TriggerAreaList[i]->getActivated() == true)
				{
					cout << "********************************************" << endl;
				}

				break;
			}
		default:
			{
			}
		}
	}
		

	
	
}
void Model_Level5::UpdatePuzzle(double dt, bool* myKeys)
{
	
}

void Model_Level5::UpdateEnemy(double dt)
{
	E_Ogre->Update(dt, level_map, goList);

	/* start set up */
	E_Ogre->StartCollisionCheck();

	/* check with wall */
	for (int i = 0; i < (*level_map).size(); i++)
	{
		(*level_map)[i]->CheckCollisionWith(E_Ogre);
	}

	for(int i = 0; i < player->coinList.size(); ++i)
	{
		if(player->coinList[i]->getActive())
		{
			E_Ogre->UpdateCoinDetection(player->coinList[i]);
		}

		else
		{
			Coin* coin = new Coin;
			coin->setActive(false);
			E_Ogre->UpdateCoinDetection(coin);
		}
	}

	/* check with all other objects */
	E_Ogre->getCollideBound()->Reset();

	//response
	E_Ogre->CollisionResponse();
}

void Model_Level5::Exit()
{
	Model::Exit();
}