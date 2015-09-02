#include "Model_Level2.h"
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
Model_Level2::Model_Level2()
{
	
}

Model_Level2::~Model_Level2()
{
}

/*********** core functions ***************/
void Model_Level2::Init()
{
	if(!initBasicsAlready)
	{
		initBasicsAlready = true;
		Model_Level::Init();

		/* timer and delay time for puzzle */
		delayTime = 0.3;
		keyPressedTimer = delayTime;

		stopGame = false;
		doorUnlocked = false;
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
		player->translate(100, 100, 1);

		//object
		spawnItems();
		InitTrigger();

		//Model_Level::mapManager.SetMap(0);	//set to map 0 first
		/* set current level map to level 1 */
		level_map = Model_Level::mapManager.GetMap(LEVEL_2);

		/* Set map scale: for camera */
		mapSize.Set(16 * 64, 13 * 64, 1);

		camera.Init(Vector3(0, -120, 0), Vector3(0, 0, -10), Vector3(0, 1, 0), m_view_width * 0.2f,  m_view_height * 0.2f
			, m_view_width, m_view_height, mapSize.x, mapSize.y);

		//Init puzzle
		puzzleManager = new PuzzleManager;
		puzzleManager->Init(MapManager::MAX_MAP);


		//object
		InitObject();
		InitPuzzles();
		
		/* Set original player position */
		originalPos.Set(100, 100, 2);

		level_state = STAGE_1;

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

void Model_Level2::InitObject()
{	
	/** Set up player **/
	ReadFromFile("Save_Load_File_lvl2.txt");

	// Post for bottom left key
	E_Ogre = new Ogre(Geometry::meshList[Geometry::GEO_CUBE], Vector3(230, 400, 0), Vector3(50, 50, 1), 0, 10, true);
	goList.push_back(E_Ogre);
}

void Model_Level2::InitTrigger()
{
	//TriggerArea puzzleActivateArea[TOTAL_PUZZLE];	//there are 3 puzzles
}

void Model_Level2::InitPuzzles()
{
	Vector3 winDimension(m_2D_view_width/2, m_2D_view_height/2, 1);

	/* State 1 */
	//note
	//pickedUpNotes[0] = false;
	puzzleNotes[0] = new Item(Geometry::meshList[Geometry::GEO_SCROLL_SEALED], Item::NOTE, true, Vector3(300, 80, 1), Vector3(35, 35, 1));
	itemList.push_back(puzzleNotes[0]);
	goList.push_back(puzzleNotes[0]);

	/* State 2 */
	puzzleActivateArea.Set(Vector3(827, 90, 1), Vector3(20, 20, 1), 5);	//press 5 times
	GameObject* obj = new GameObject;
	obj->Set("Debug cube for trigger area", Geometry::meshList[Geometry::GEO_DEBUG_CUBE], NULL, false, false);
	obj->translateObject(puzzleActivateArea.position);
	obj->scaleObject(puzzleActivateArea.scale.x, puzzleActivateArea.scale.y, 1);
	goList.push_back(obj);

	//key: to unlock door 0
	puzzleKeys[0] = new Item(Geometry::meshList[Geometry::GEO_KEYY], Item::KEY, true, Vector3(200, 500, 0), Vector3(35, 35, 1));
	puzzleKeys[0]->setActive(false);
	//pickedUpKeys[0] = false;
	goList.push_back(puzzleKeys[0]);
	itemList.push_back(puzzleKeys[0]);

	//note
	//pickedUpNotes[1] = false;
	puzzleNotes[1] = new Item(Geometry::meshList[Geometry::GEO_SCROLL_SEALED], Item::NOTE, true, Vector3(300, 80, 1), Vector3(35, 35, 1));
	itemList.push_back(puzzleNotes[1]);
	puzzleNotes[1]->setActive(false);
	goList.push_back(puzzleNotes[1]);

	//player 2 zone
	stage2Area.Set(Vector3(827, 90, 1), Vector3(120, 120, 1), Collision::BOX);	
	obj = new GameObject;
	obj->Set("Debug cube for trigger area", Geometry::meshList[Geometry::GEO_DEBUG_CUBE], NULL, false, false);
	obj->translateObject(stage2Area.position);
	obj->scaleObject(stage2Area.scale.x, stage2Area.scale.y, 1);
	goList.push_back(obj);

	/* State 3 */
	stage3Area.Set(Vector3(877, 190, 1), Vector3(120, 120, 1), Collision::BOX);	
	obj = new GameObject;
	obj->Set("Debug cube for trigger area", Geometry::meshList[Geometry::GEO_DEBUG_CUBE], NULL, false, false);
	obj->translateObject(stage3Area.position);
	obj->scaleObject(stage3Area.scale.x, stage3Area.scale.y, 1);
	goList.push_back(obj);

	//lever to remove traps
	leverClose = new TriggerObject(Geometry::meshList[Geometry::GEO_NOTTRIGGER], TriggerObject::FIRETRIGGER, Vector3(60, 654, 1), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	goList.push_back(leverClose);

	leverOpen = new TriggerObject(Geometry::meshList[Geometry::GEO_ISTRIGGER], TriggerObject::FIRETRIGGER, Vector3(60, 654, 1), Vector3(45, 45, 1), 0, true, *sfx_man, player);
	leverOpen->setActive(false);
	goList.push_back(leverOpen);

	/* State 4 */
	spikeTrap = new GameObject(Geometry::meshList[Geometry::GEO_CUBE], Vector3(80, 403, 1), Vector3(100, 100, 1), true);
	goList.push_back(spikeTrap);

	//doors
	doors[0] = new GameObject(Geometry::meshList[Geometry::GEO_DOORY], Vector3(150, 654, 1), Vector3(50, 90, 1), true);
	goList.push_back(doors[0]);

	doors[1] = new GameObject(Geometry::meshList[Geometry::GEO_DOORY], Vector3(579, 598, 1), Vector3(50, 90, 1), true);
	goList.push_back(doors[1]);

	//key: to unlock door 1
	puzzleKeys[1] = new Item(Geometry::meshList[Geometry::GEO_KEYY], Item::KEY, true, Vector3(80, 483, 0), Vector3(35, 35, 1));
	goList.push_back(puzzleKeys[1]);
	//pickedUpKeys[1] = false;
	itemList.push_back(puzzleKeys[1]);

	/***************** TUTORIAL *************************/
	/* Teach noobs how to pick up note */
	tutorialUI.SetActive(true);
	tutorialUI.SetMessage("Pick up the clue left on the floor. Press B to view it");
}

void Model_Level2::spawnItems()
{
}

void Model_Level2::Update(double dt, bool* myKeys, Vector3 mousePos, StateManager::STATES currentState)
{
	/* parent class update */
	Model_Level::Update(dt, myKeys, mousePos, currentState);
	//cout << player->getPosition() << endl;

	if(keyPressedTimer < delayTime)
		keyPressedTimer += dt;
	
	/* Update game */
	UpdateGame(dt, myKeys);
}

void Model_Level2::UpdateGame(double dt, bool* myKeys)
{
	if (stopGame == false)
	{
		// Sound - ambience
		sfx_man->play_ambience();

		//Update enemy
		UpdateEnemy(dt);

		/* Update player */
		player->Update(dt, myKeys);

		/* Update puzzle (put above check collision with door since we need response for door) */
		UpdatePuzzle(dt, myKeys);

		if(myKeys[KEY_K])
		{
			player->Translate(Vector3(659, 389, 0));
		}

		player->dropItem(dt, item, myKeys);

		/* Collision checking */
		UpdateCollision();

		/** If stop game **/
		if (player->getHealth() == 0)
		{
			stopGame = true;
		}
	}
}

void Model_Level2::UpdateCollision()
{
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

		/** Check collision with doors */
		for(int i = 0; i < 2; ++i)
		{
			if(doors[i]->getActive())	//only check collide if active
				player->CollisionCheck(doors[i]);
		}

		/*** Change to next level ***/
		if (player->CollisionCheck(staircase))
		{
			goNextLevel = true;
			Model_Level::setNextLevel(goNextLevel);
			mapTimer = 0;
		}

		/** Reset collision **/
		player->getCollideBound()->Reset();

		/** Collision response (if any) **/
		player->CollisionResponse();	//translate to new pos if collides
}

void Model_Level2::UpdatePuzzle(double dt, bool* myKeys)
{
	/* Get damaged by spike trap */
	if(spikeTrap->getActive())
	{
		if(player->QuickAABBDetection(spikeTrap))
		{
			player->setHealth(player->getHealth() - SPIKE_DMG);
		}
	}

	/* See puzzle message */
	if(puzzleMsgTimer < puzzleMsgTime)
	{
		puzzleMsgTimer += dt;
	}
	else
	{
		puzzleMsgTimer = 0.0;
		if(myKeys[KEY_B])
		{
			if(puzzleMessage.getActive())
			{
				puzzleMessage.SetActive(false);
			}
			else
			{
				/* if note that we pick up is note 0 */
				if(!puzzleNotes[0]->getItemFloor())
				{
					puzzleMessage.SetMessage("Blueberry and Corn");
					puzzleMessage.SetActive(true);
				}
				/* if note that we pick up is note 0 */
				else if(!puzzleNotes[1]->getItemFloor())
				{
					puzzleMessage.SetMessage("Lever");
					puzzleMessage.SetActive(true);
				}
			}
		}
	}

	switch (level_state)
	{
	case STAGE_1:
		{
			tutorialUI.SetActive(true);	//only show when prompted

			/* Check if collide with note 1 */
			if(player->pickUp(puzzleNotes[0], myKeys))
			{
				level_state = STAGE_2;
				tutorialUI.SetActive(false);	//pick up then can disable tutorial
			}

			break;
		}
	case STAGE_2:
		{
			/* If in stage 2 zone: show tutorial to spam to pick up */
			if(stage2Area.QuickAABBDetection(player->getCollideBound()))
			{
				tutorialUI.SetActive(true);
				tutorialUI.SetMessage("Congrats on finding where the clue leads!/ Press E multiple times to shake the box/ and get the contents");
			}

			/* see if click at trigger area to get hidden key and message */
			if(keyPressedTimer >= delayTime)
			{
				if(myKeys[KEY_E])
				{
					keyPressedTimer = 0.0;
					puzzleActivateArea.QuickAABBDetection(player->getCollideBound(), true);
				}
			}

			/* if activated: get key and puzzle */
			if(puzzleActivateArea.getActivated())
			{
				//disable tutorial
				tutorialUI.SetActive(false);

				puzzleNotes[1]->setActive(true);	//must be active before can add
				player->getInventory()->addItem(puzzleNotes[1]);	//note 2
				puzzleKeys[0]->setActive(true);	//must be active before can add
				player->getInventory()->addItem(puzzleKeys[0]);	//key 1
				level_state = STAGE_3;
			}

			break;
		}
	case STAGE_3:
		{
			/* If in stage 2 zone: show tutorial to spam to pick up */
			if(stage3Area.QuickAABBDetection(player->getCollideBound()))
			{
				tutorialUI.SetActive(true);
				tutorialUI.SetMessage("Unlock the door with the key");
			}
			else
				tutorialUI.SetActive(false);

			/* check if can open door */
			if(player->QuickAABBDetection(doors[0]) && !puzzleKeys[0]->getItemFloor())	//picked up key 0?
			{
				/* Open only when holding key */
				if(player->getInventory()->currentItemID() == Item::KEY)
				doors[0]->setActive(false);
				sfx_man->play_unlock();
			}

			/* if lever pulled: go stage 4 */
			if(keyPressedTimer >= delayTime)
			{
				if(myKeys[KEY_E])
				{
					/* if collide with lever and pressed E: activated lever */
					if(player->QuickAABBDetection(leverClose))
					{
						keyPressedTimer = 0.0;
						leverClose->setActive(false);
						leverOpen->setActive(true);

						spikeTrap->setActive(false);
						level_state = STAGE_4;
					}
				}
			}

			break;
		}
	case STAGE_4:
		{
			/* see if pick up key 1 */
			if(keyPressedTimer >= delayTime)
			{
				if(myKeys[KEY_E])
				{
					if(player->pickUp(puzzleKeys[1], myKeys))
					{
						keyPressedTimer = 0.0;
					}
				}
			}

			/* if got key: can unlock door */
			if(player->QuickAABBDetection(doors[1]) && !puzzleKeys[1]->getItemFloor())	//picked up key 0?
			{
				doors[1]->setActive(false);
				sfx_man->play_unlock();
			}

			break;
		}
	}
}

void Model_Level2::UpdateEnemy(double dt)
{

	/*if(player->getInvisible())
	{
		E_Ogre->setDetectedPlayer(false);
	}*/

	E_Ogre->Update(dt, level_map, goList);

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

void Model_Level2::Exit()
{
	Model::Exit();
}