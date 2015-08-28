//#include "model_2D.h"
//#include "GL\glew.h"
//#include "shader.hpp"
//#include "MeshBuilder.h"
//#include "Controller.h"
//#include "Utility.h"
//#include "LoadTGA.h"
//#include "LoadHmap.h"
//#include "SpriteAnimation.h"
//#include <sstream>
//
//#include "StaticObject.h"
////(tip) If create bullet, bullet class has a static TRS so that TRS update with current bullet pos
//
///*********** constructor/destructor ***************/
//Model_menu::Model_menu()
//{
//	
//}
//
//Model_menu::~Model_menu()
//{
//}
//
///*********** core functions ***************/
//void Model_menu::Init()
//{
//	Model_Level::Init();
//	
//	InitMaps();	//create map for all levels
//	Model_Level::mapManager.SetMap(0);	//set to map 0 first
//
//	/* timer and delay time */
//	delayTime = 0.3;
//	keyPressedTimer = delayTime;
//
//	stopGame = false;
//	doorUnlocked = false;
//	haveFire = true;
//	Timer = 0;
//	mapTimer = 0;
//
//	//object
//	InitObject();
//	InitSprites();
//	spawnItems();
//	InitTrigger();
//
//	//UI
//	InitUI();
//
//	//Init puzzle
//	puzzleManager = new PuzzleManager;
//	puzzleManager->Init(MapManager::MAX_MAP);
//	InitPuzzles();
//}
//
//void Model_menu::InitObject()
//{	
//	/** Set up player **/
//	ReadFromFile("Save_Load_File.txt");
//
//	E_Ogre = new Ogre(Geometry::meshList[Geometry::GEO_CUBE], Vector3(700, 600, 0), Vector3(50, 50, 1), 0, 10, true);
//	goList.push_back(E_Ogre);
//
//	/** init **/
//	for(std::vector<GameObject*>::iterator it = goList.begin(); it != goList.end(); ++it)
//	{
//		Object *go = (Object *)*it;
//		go->Init();
//	}
//}
//
//void Model_menu::InitTrigger()
//{
//	triggerObject[0] = new TriggerObject(Geometry::meshList[Geometry::GEO_NOTTRIGGER], TriggerObject::NOTTRIGGERED, Vector3(750, 560, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
//	goList.push_back(triggerObject[0]);
//
//	triggerObject[1] = new TriggerObject(Geometry::meshList[Geometry::GEO_ISTRIGGER], TriggerObject::ISTRIGGERED, Vector3(750, 560, -3), Vector3(45, 45, 1), 0, true, *sfx_man, player);
//	goList.push_back(triggerObject[1]);
//
//	//Fire
//	triggerObject[2] = new TriggerObject(Geometry::meshList[Geometry::GEO_FIRE], TriggerObject::FIRE, Vector3(550, 688, 0), Vector3(30, 158, 1), 0, true, *sfx_man, player);
//	triggerObject[2]->storeSpriteAnimation("fire trap", 1, 7, "Image//Sprites//fire.tga");
//	triggerObject[2]->processSpriteAnimation(TriggerObject::FIRE, 0.8f, 1, 0, 6, 0, 1, false);
//	goList.push_back(triggerObject[2]);
//
//	triggerObject[3] = new TriggerObject(Geometry::meshList[Geometry::GEO_FIRE], TriggerObject::FIRE, Vector3(680, 688, 0), Vector3(30, 158, 1), 0, true, *sfx_man, player);
//	triggerObject[3]->storeSpriteAnimation("fire trap", 1, 7, "Image//Sprites//fire.tga");
//	triggerObject[3]->processSpriteAnimation(TriggerObject::FIRE, 1.f, 1, 0, 6, 0, 1, false);
//	goList.push_back(triggerObject[3]);
//
//	//Arrow trap
//	//triggerObject[4] = new TriggerObject(Geometry::meshList[Geometry:::])
//}
//
//void Model_menu::InitUI()
//{
//	//Vector3 winDimension(m_2D_view_width/2, m_2D_view_height/2, 1);
//
//	///* background main menu */
//	//UI_Object* obj;
//	//obj = new UI_Object;
//	//obj->Init(Geometry::meshList[Geometry::GEO_BOTTOM], winDimension, Vector3(m_2D_view_width, m_2D_view_height, 1), "", UI_Object::MAIN_MENU_BACKGROUND, true);
//	//UI_List.push_back(obj);
//
//	///* UI Objects */
//	//Controller::mouse_current_x;
//	//Vector3 mousePos(Controller::mouse_current_x, Controller::mouse_current_y, 3);
//	//cursor.Init(Geometry::meshList[Geometry::GEO_BACK], mousePos, Vector3(5, 5, 1), "", UI_Object::MOUSE_CURSOR, true);
//	//UI_List.push_back(&cursor);
//
//	//start_Game.Init(Geometry::meshList[Geometry::GEO_CUBE], Vector3(winDimension.x, winDimension.y + 11, 1.1), Vector3(40, 15, 1), "Start Game", UI_Object::BUTTON, true);
//	//UI_List.push_back(&start_Game);	
//
//	//instruction.Init(Geometry::meshList[Geometry::GEO_CUBE], Vector3(winDimension.x, winDimension.y - 11, 1.1), Vector3(40, 15, 1), "Instructions", UI_Object::BUTTON, true);
//	//UI_List.push_back(&instruction);
//}
//
//void Model_menu::InitSprites()
//{
//	//Player sprites
//	player->storeSpriteAnimation("black guard", 21, 13, "Image//Sprites//guard.tga");
//	player->processSpriteAnimation(Player::UP, 0.5f, 0, 8, 8, 8, 1);
//	player->processSpriteAnimation(Player::DOWN, 0.5f, 0, 10, 8, 10, 1);
//	player->processSpriteAnimation(Player::LEFT, 0.5f, 0, 9, 8, 9, 1);
//	player->processSpriteAnimation(Player::RIGHT, 0.5f, 0, 11, 8, 11, 1);
//	player->processSpriteAnimation(Player::ATTACKUP, 0.5f, 0, 4, 7, 4, 1);
//	player->processSpriteAnimation(Player::ATTACKDOWN, 0.5f, 0, 6, 7, 6, 1);
//	player->processSpriteAnimation(Player::ATTACKLEFT, 0.5f, 0, 5, 7, 5, 1);
//	player->processSpriteAnimation(Player::ATTACKRIGHT, 0.5f, 0, 7, 7, 7, 1);
//}
//
//void Model_menu::InitPuzzles()
//{
//	puzzleManager->addTextPuzzle(MapManager::MAP1, "test1");
//	puzzleManager->addTextPuzzle(MapManager::MAP1, "test2");
//	puzzleManager->addPicturePuzzle(MapManager::MAP1, "Image//Sprites//guard.tga");
//	puzzleManager->addTextPuzzle(MapManager::MAP2, "test3");
//	puzzleManager->addTextPuzzle(MapManager::MAP3, "test4");
//
//	/*cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
//	puzzleManager->goToNextPart();
//	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
//	puzzleManager->goToNextPart();
//	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
//	puzzleManager->goToNextPart();
//	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
//	puzzleManager->goToNextPart();
//	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;
//	puzzleManager->goToNextPart();
//	cout << puzzleManager->getCurrentPuzzle()->getTextPuzzle() << endl;*/
//}
//
//void Model_menu::spawnItems()
//{
//	item = new Item(Geometry::meshList[Geometry::GEO_KEYY], Item::KEY, true, Vector3(200, 500, 0), Vector3(35, 35, 1));
//	goList.push_back(item);
//	itemList.push_back(item);
//}
//
//void Model_menu::InitMaps()
//{
//	//Model_Level::mapManager.CreateMap(MapManager::MAP1, 32, 25, 32, "Image//Map//test.csv", Geometry::meshList[Geometry::GEO_DUNGEONTILE]);
//	Model_Level::mapManager.CreateMap(MapManager::MAP1, Map::FLOORMAP, 16, 13, 64, "Image//Map//tempfloor.csv", Geometry::meshList[Geometry::GEO_TEMPFLOOR], false);
//	//Model_Level::mapManager.CreateMapFloor(MapManager::MAP1, 32, 25, 32, Geometry::meshList[Geometry::GEO_JINFLOOR]);
//	Model_Level::mapManager.AddRear(MapManager::MAP1, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//map1_Tile Layer 1.csv", Geometry::meshList[Geometry::GEO_DUNGEONTILE]);
//	Model_Level::mapManager.AddRear(MapManager::MAP1, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//map1_Tile Layer 2.csv", Geometry::meshList[Geometry::GEO_TILESET1]);
//	//Model_Level::mapManager.AddRear(MapManager::MAP1, Map::NOCOLLISIONMAP, 32, 25, 32, "Image//Map//map1_Tile Layer 3.csv", Geometry::meshList[Geometry::GEO_TILESET1], false);
//	Model_Level::mapManager.CreateMap(MapManager::MAP2, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//MapDesign_lvl1.csv", Geometry::meshList[Geometry::GEO_TILEMAP]);
//	Model_Level::mapManager.CreateMap(MapManager::MAP3, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//MapDesign_lvl2.csv", Geometry::meshList[Geometry::GEO_TILEMAP]);
//}
//
//void Model_menu::Update(double dt, bool* myKeys, Vector3 mousePos)
//{
//	/* parent class update */
//	Model::Update(dt, myKeys, mousePos);
//
//	if(keyPressedTimer < delayTime)
//		keyPressedTimer += dt;
//	
//	/* Update based on states */
//	switch (stateManager.GetState())
//	{
//	case StateManager::MAIN_MENU:
//			//UpdateMainMenu(dt, myKeys, mousePos.x,  mousePos.y);
//			break;
//	case StateManager::GAME:
//			UpdateGame(dt, myKeys);
//			break;
//	case StateManager::INSTRUCTION:
//			//UpdateInstructions(dt, myKeys, mousePos.x, mousePos.y);
//			break;
//	}
//
//	/* If in transition */
//	if (stateManager.isTransition())
//	{
//		stateManager.UpdateTransitionTime(dt);
//	}
//}
//
//void Model_menu::UpdateGame(double dt, bool* myKeys)
//{
//	if(stopGame == false)
//	{
//	// Sound - ambience
//		sfx_man->play_ambience();
//
//		//Update enemy
//		UpdateEnemy(dt);
//
//		/* Update player */
//		player->Update(dt, myKeys);
//
//		//Update the traps
//		UpdateTraps(dt, myKeys);
//
//		if(myKeys[KEY_K])
//		{
//			player->Translate(Vector3(659, 389, 0));
//		}
//
//	player->dropItem(dt, item, myKeys);
//	
//	/* check collision with object */
//	//start: Set up collision bound before checking with the others
//	player->StartCollisionCheck();
//
//		//getCamera()->position.Set(player->getPosition().x-500, player->getPosition().y-400, 1);
//		//getCamera()->target.Set(player->getPosition().x-500, player->getPosition().y-400, 0);
//
//		//cout << player->getPosition() << endl;
//
//		for (int i = 0; i < Model_Level::mapManager.GetCurrentMap()->size(); i++)
//		{
//			if ((*Model_Level::mapManager.GetCurrentMap())[i]->getMapType() == Map::COLLISIONMAP)
//			{
//				//(*Model_Level::mapManager.GetCurrentMap())[i]->getWalkable(player->getPosition().x, player->getPosition().y);
//			}
//		}
//
//		/* check collision with object */
//		//start: Set up collision bound before checking with the others
//		player->StartCollisionCheck();
//
//		/* check collision with map */
//		for (int i = 0; i < Model_Level::mapManager.GetCurrentMap()->size(); i++)
//		{
//			if ((*Model_Level::mapManager.GetCurrentMap())[i]->getMapType() == Map::COLLISIONMAP)
//			{
//				(*Model_Level::mapManager.GetCurrentMap())[i]->CheckCollisionWith(player);
//			}
//		}
//
//		if(door->getActive())
//		{
//			if(player->CollisionCheck(door))
//			{
//				if(doorUnlocked)
//				{
//					door->setActive(false);
//				}
//			}
//		}
//
//	player->dropItem(dt, item, myKeys);
//		mapTimer += dt;
//		if (player->CollisionCheck(staircase))
//		{
//			if(mapTimer > 5)
//			{
//				Model_Level::mapManager.ChangeNextMap();
//				mapTimer = 0;
//			}
//		}
//
//		//player->dropItem(dt, item, myKeys);
//
//		player->getCollideBound()->Reset();
//
//		/* Collision response */
//		player->CollisionResponse();	//translate to new pos if collides
//
//
//		/* Test pick up items */
//		for(int i = 0; i < itemList.size(); ++i)
//		{
//			if(player->pickUp(itemList[i], myKeys))	//if successfully pick up
//			{
//				//if item is key
//				//cout << itemList[i]->getItemID() << endl;
//				if(itemList[i]->getItemID() == Item::KEY)
//				{
//					doorUnlocked = true;
//				}
//			}
//		}
//
//		player->useItem(myKeys);
//
//		/* Update target */
//		camera.target = camera.position;
//		camera.target.z -= 10;
//
//		/* Press space to go back main menu */
//		if(myKeys[KEY_SPACE] && keyPressedTimer >= delayTime)
//		{
//			keyPressedTimer = 0.0;
//			//stateManager->ChangeState(stateManager->MAIN_MENU);
//		}
//
//		/* Key Q to open puzzle */
//		static bool ButtonQState = false;
//		if (!ButtonQState && myKeys[KEY_Q])
//		{
//			ButtonQState = true;
//			std::cout << "QBUTTON DOWN" << std::endl;
//			puzzleOpen = true;
//		}
//		else if (ButtonQState && !(myKeys[KEY_Q]))
//		{
//			ButtonQState = false;
//			std::cout << "QBUTTON UP" << std::endl;
//			puzzleOpen = false;
//		}
//
//		/* Load/change map */
//		//Key B to move to next map (RP)
//		static bool ButtonBState = false;
//		if (!ButtonBState && myKeys[KEY_B])
//		{
//			ButtonBState = true;
//			std::cout << "BBUTTON DOWN" << std::endl;
//			//stateManager->ChangeState(StateManager::MAIN_MENU);
//			//Model_Level::mapManager.ChangeNextMap();
//			puzzleManager->goToNextPart();
//		}
//		else if (ButtonBState && !(myKeys[KEY_B]))
//		{
//			ButtonBState = false;
//			std::cout << "BBUTTON UP" << std::endl;
//		}
//
//		if(player->getHealth() == 0)
//		{
//			stopGame = true;
//		}
//	}
//} 
//
//void Model_menu::UpdateTraps(double dt, bool* myKeys)
//{
//	Timer += dt;
//	/* check with trigger objects fire */
//	for(int i = 0; i < 4; i++)
//	{
//		triggerObject[i]->Update(dt, myKeys);
//	}
//
//	if(triggerObject[0]->getTriggered() == true) //If lever is switched off
//	{
//		triggerObject[0]->setActive(false); //to change lever position
//		triggerObject[1]->setActive(true); 
//		triggerObject[2]->setActive(false); //fire
//		triggerObject[3]->setActive(false); //fire
//		haveFire = false; //do not render fire
//	}
//	else 
//	{
//		triggerObject[0]->setActive(true); 
//		triggerObject[1]->setActive(false);
//		triggerObject[2]->setActive(true); //fire on
//		triggerObject[3]->setActive(true); //fire on
//		haveFire = true;
//	}
//
//	if(Timer >= 0.5)
//	{
//		if(player->QuickAABBDetection(triggerObject[2]) && haveFire == true || player->QuickAABBDetection(triggerObject[3]) && haveFire == true)
//		{
//			player->setHealth(player->getHealth() - 15);
//			player->Translate(player->getPosition() - 45);
//			Timer = 0;
//		}
//	}
//}
//
//void Model_menu::UpdateEnemy(double dt)
//{
//	E_Ogre->Update(dt,mapManager, goList);
//
//	/* start set up */
//	E_Ogre->StartCollisionCheck();
//
//	/* check with wall */
//	for (int i = 0; i < Model_Level::mapManager.GetCurrentMap()->size(); i++)
//	{
//		(*Model_Level::mapManager.GetCurrentMap())[i]->CheckCollisionWith(player);
//	}
//
//	/* check with all other objects */
//	E_Ogre->getCollideBound()->Reset();
//
//	//response
//	E_Ogre->CollisionResponse();
//}
//
////void Model_menu::UpdateInstructions(double dt, bool* myKeys, double mouse_x, double mouse_y)
////{
////	/* Update cursor */
////	cursor.Follow(mouse_x, mouse_y);	//hard coded console height
////
////	/* Check collide */
////	if(cursor.QuickAABBDetection(&go_back) && myKeys[KEY_LMOUSE])	//go back to main menu
////	{
////		go_back.SetActive(false);
////		start_Game.SetActive(true);
////		instruction.SetActive(true);
////		stateManager->ChangeState(stateManager->MAIN_MENU);
////	}
////}
//
////void Model_menu::UpdateMainMenu(double dt, bool* myKeys, double mouse_x, double mouse_y)
////{
////	/* Update cursor */
////	cursor.Follow(mouse_x, mouse_y);	//hard coded console height
////
////	/* Check collide */
////	if(cursor.QuickAABBDetection(&start_Game) && myKeys[KEY_LMOUSE])	//pressed start game button
////	{
////		start_Game.SetActive(false);
////		instruction.SetActive(false);
////		go_back.SetActive(true);
////		stateManager->ChangeState(stateManager->GAME);
////	}
////	else if(cursor.CheckCollision(instruction) && myKeys[KEY_LMOUSE])	//pressed instructions
////	{
////		start_Game.SetActive(false);
////		instruction.SetActive(false);
////		go_back.SetActive(true);
////		stateManager->ChangeState(stateManager->INSTRUCTION);
////	}
////}
//
//bool getWordFromString(string& sentence, string& word, char min, char max, int& index)
//{
//	bool returnMe = false;
//	word = "";
//	for(; index < sentence.length(); ++index)	//loop through
//	{
//		/* get word based on min and max har*/
//		
//		if(sentence[index] >= min && sentence[index] <= max)	
//		{
//			word += sentence[index];	//get the whole word
//			returnMe = true;
//		}
//		else if(returnMe)	//if reach end of min max char
//			return true;
//		
//		if(index == sentence.length() - 1 && returnMe)	//last char and is still min max range
//		{
//			++index;
//		
//			return true;
//		}
//	}
//	return false;
//}
//
//float stringTofloat(string& number)
//{
//	/* interpret any ascii at front not 48-57 as minus sign */
//	float returnVal = 0;
//	int stopIndex = 0;
//	float multiplier = 1;
//
//	if(number[0] == '-')
//		stopIndex = 1;
//
//	/* check if got floating point */
//	int floatPoint_total = -1;	//get index
//	for(int i = number.length() - 1; i >= stopIndex; --i)
//	{
//		if(number[i] == '.')	//floating point
//		{
//			floatPoint_total = (number.length() - 1) - i;
//			break;
//		}
//	}
//
//	/* set the multiplier to starting value */
//	if(floatPoint_total != -1)
//	{
//		for(int i = 0; i < floatPoint_total; ++i)
//			multiplier /= 10.f;
//	}
//
//
//	/* calculate num */
//	for(int i = number.length() - 1; i >= stopIndex; --i)
//	{
//		if(number[i] >= 48 && number[i] <= 57)
//		{
//			returnVal += (number[i] - 48) * multiplier;
//			multiplier *= 10;
//		}
//	}
//
//	if(stopIndex > 0)
//	{
//		return returnVal * -1;
//	}
//	else
//	{
//		return returnVal;
//
//	}
//}
//
//void Model_menu::Exit()
//{
//	Model::Exit();
//}