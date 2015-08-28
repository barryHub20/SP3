#include "Model_Level.h"
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
MapManager Model_Level::mapManager;
Player* Model_Level::player = NULL;
// Player* Model_Level::player = NULL;
bool Model_Level::init_Already = false;
bool Model_Level::goNextLevel = false;
bool Model_Level::goPreviousLevel = false;

/*********** constructor/destructor ***************/
Model_Level::Model_Level()
{
	init_Already = false;
}

Model_Level::~Model_Level()
{
}

/*********** core functions ***************/
void Model_Level::Init()
{
	if(!init_Already)
	{
		Model::Init();

		/* game state setup */
		mapManager.Init();
		InitMaps();

		goNextLevel = goPreviousLevel = false;
		init_Already = true;

	
		current_model = 0;

		stateManager.ChangeState(StateManager::MAIN_MENU);
	}
}

void Model_Level::InitMaps()
{
	/** Create maps for all levels **/
	//Model_Level::mapManager.CreateMap(MapManager::MAP1, 32, 25, 32, "Image//Map//test.csv", Geometry::meshList[Geometry::GEO_DUNGEONTILE]);
	Model_Level::mapManager.CreateMap(MapManager::MAP1, Map::FLOORMAP, 16, 13, 64, "Image//Map//tempfloor.csv", Geometry::meshList[Geometry::GEO_TEMPFLOOR], false);
	//Model_Level::mapManager.CreateMapFloor(MapManager::MAP1, 32, 25, 32, Geometry::meshList[Geometry::GEO_JINFLOOR]);
	Model_Level::mapManager.AddRear(MapManager::MAP1, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//map1_Tile Layer 1.csv", Geometry::meshList[Geometry::GEO_DUNGEONTILE]);
	Model_Level::mapManager.AddRear(MapManager::MAP1, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//map1_Tile Layer 2.csv", Geometry::meshList[Geometry::GEO_TILESET1]);
	//Model_Level::mapManager.AddRear(MapManager::MAP1, Map::NOCOLLISIONMAP, 32, 25, 32, "Image//Map//map1_Tile Layer 3.csv", Geometry::meshList[Geometry::GEO_TILESET1], false);
	//Model_Level::mapManager.CreateMap(MapManager::MAP2, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//MapDesign_lvl1.csv", Geometry::meshList[Geometry::GEO_TILEMAP]);
	//Model_Level::mapManager.CreateMap(MapManager::MAP3, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//MapDesign_lvl2.csv", Geometry::meshList[Geometry::GEO_TILEMAP]);
	Model_Level::mapManager.CreateMap(MapManager::MAP2, Map::FLOORMAP, 16, 13, 64, "Image//Map//tempfloor.csv", Geometry::meshList[Geometry::GEO_TEMPFLOOR], false);
	//Model_Level::mapManager.CreateMapFloor(MapManager::MAP1, 32, 25, 32, Geometry::meshList[Geometry::GEO_JINFLOOR]);
	//Model_Level::mapManager.AddRear(MapManager::MAP1, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//map1_Tile Layer 1.csv", Geometry::meshList[Geometry::GEO_DUNGEONTILE]);
	Model_Level::mapManager.AddRear(MapManager::MAP2, Map::COLLISIONMAP, 32, 25, 32, "Image//Map//map1_Tile Layer 2.csv", Geometry::meshList[Geometry::GEO_TILESET1]);
	
}

void Model_Level::Update(double dt, bool* myKeys, Vector3 mousePos)
{
}

bool Model_Level::ReadFromFile(char* text)
{
	string sentence, object_word, case_word;

	int index;
	bool tmp_active;
	Vector3 tmp_pos, tmp_scale;
	float tmp_angle;
	float tmp_floatArr[3];	//to store tmp float variable for vector3
	string tmp_string;		//to store tmp string values
	int tmp_playerID;	//for goalposts

	string active_txt = "active";
	string pos_txt = "pos";
	string scale_txt = "scale";

	ifstream myFile(text);

	if(myFile.fail())
	{
		cout << "Failed to find the text file, the text file is empty" << endl;
		return false;
	}

	while(!myFile.eof())
	{
		std::getline(myFile, sentence, '\n');
		cout << sentence << endl;
		index = 0;
		
		/******************** if is comment/non-caps for first char, skip ********************/
		if(sentence.length() > 0 && !(sentence[index] >= 65 && sentence[index] <= 90) )	
		{
			continue;
		}
		/******************** Get KEYWORD so we know what object to create ********************/
		if( !getWordFromString(sentence, object_word, 65, 90, index) )
		{
			cout << "Line undefined: going to next line" << endl;
			continue;
		}
		
		/************************** get variable words **************************/
		while(index < sentence.length())
		{
			case_word = "";	//the keyword

			/* got word */
			if(getWordFromString(sentence, case_word, 97, 122, index) )
			{
				bool start = false;	//start assigning
				string variables  = "";	//(x, y, z)

				/* get the words inside the bracket */
				for(;index < sentence.length(); ++index)
				{
					if(sentence[index] == ')')
					{
						break;
					}

					if(start)
					{
						variables += sentence[index];
					}

					if(sentence[index] == '(')
					{
						start = true;
					}
				}

				/************************** get number/s from brackets, eg. (x, y, z) **************************/
				string value = "";
				bool correct = false;
				int numIndex = 0;	//index
				unsigned counter = 0; //if is Vector3, count 3 times(take in value) and break, if is float, just count one time and break
				while(numIndex < variables.length())
				{
					if(case_word == active_txt)	//active: T/F
					{
						correct = getWordFromString(variables, value, 60, 90, numIndex);
					}
					else	//the rest: minus sign and 0-9
					{
						correct = getWordFromString(variables, value, 45, 57, numIndex);
					}

					if(!correct)
					{
						cout << "Unidentified variable: moving on to next variable" << endl;
						break;
					}

					/* get values from txt file */
					if(case_word == active_txt)	//keyword is active: special case immediately assign here
					{
						tmp_string = value;
					}
					else						//keyword is float values
					{
						tmp_floatArr[counter] = stringTofloat(value);
						++counter;
					}
				}

				/** Assign tmp values here **/
				if(case_word == pos_txt)
				{
					tmp_pos.Set(tmp_floatArr[0], tmp_floatArr[1], tmp_floatArr[2]);
				}
				else if(case_word == scale_txt)
				{
					tmp_scale.Set(tmp_floatArr[0], tmp_floatArr[1], tmp_floatArr[2]);
				}
				else if(case_word == active_txt)
				{
					if(tmp_string == "T")	//true
					{
						tmp_active = true;
					}
					else
					{
						tmp_active = false;
				
					}
				}
			}
		}
		 
		/************************** Create Relevant object **************************/
		if(object_word == "PLAYER")
		{
			if(player == NULL)
			{
				player = new Player(Geometry::meshList[Geometry::GEO_CUBE], Vector3(tmp_pos.x, tmp_pos.y, 0), Vector3(tmp_scale.x, tmp_scale.y, 1), 0, 10, true, *sfx_man);
				player->getInventory()->Set(0.43f, 0.1f, m_2D_view_width, m_2D_view_height, 0.97f, 0.02f);
			}
		}

		if(object_word == "DOOR")
		{
			door = new TriggerObject(Geometry::meshList[Geometry::GEO_DOORY], TriggerObject::DOOR, Vector3(tmp_pos.x, tmp_pos.y, 0), Vector3(tmp_scale.x, tmp_scale.y, 1), 0, true, *sfx_man, player);
			goList.push_back(door);
		}
		if (object_word == "RSTAIRCASEUP")
		{
			staircase = new TriggerObject(Geometry::meshList[Geometry::GEO_RSTAIRCASEUP], TriggerObject::TRIGGERWHENCOLLIDE, Vector3(tmp_pos.x, tmp_pos.y, 0), Vector3(tmp_scale.x, tmp_scale.y, 1), 0, true, *sfx_man, player);
			goList.push_back(staircase);
		}
		if(object_word == "HPOTION")
		{
			item = new Item(Geometry::meshList[Geometry::GEO_HPOTION], Item::H_POTION, true, Vector3(tmp_pos.x, tmp_pos.y, 0), Vector3(tmp_scale.x, tmp_scale.y, 1));
			goList.push_back(item);
			itemList.push_back(item);
		}
		if(object_word == "SPOTION")
		{
			item = new Item(Geometry::meshList[Geometry::GEO_SPOTION], Item::S_POTION, true, Vector3(tmp_pos.x, tmp_pos.y, 0), Vector3(tmp_scale.x, tmp_scale.y, 1));
			goList.push_back(item);
			itemList.push_back(item);
		}
	}
	myFile.close();
	return true;
}

void Model_Level::Exit()
{
	Model::Exit();
}

/*********** getter/setter ***************/
vector<GameObject*>* Model_Level::getObject(){return &goList;}
vector<UI_Object*>* Model_Level::getUIList(){return &UI_List;}
vector<Item*>* Model_Level::getItemList(){return &itemList;}
//bool  Model_Level::goNextLevel(){
//bool  Model_Level::goPreviousLevel()
bool Model_Level::NextLevel(){return goNextLevel;}
bool Model_Level::PreviousLevel(){return goPreviousLevel;}

void Model_Level::setNextLevel(bool i)
{
	goNextLevel = i;
}

void Model_Level::setPreviousLevel(bool i)
{
	goNextLevel = i;
}

vector<Map*>* Model_Level::getLevelMap()
{
	return level_map;
}