#include "Model_Terrain.h"
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
ModelTerrain::ModelTerrain()
{
	minimap = NULL;
}

ModelTerrain::~ModelTerrain()
{
}

/*********** core functions ***************/
void ModelTerrain::Init()
{
	m_view_width = 180.f;
	m_view_height = 120.f;

	worldDimension.Set(800, 600, 10);

	Model::Init();

	//perspective
	fovAngle = DEFAULT_FOV;
	FovRate = 1.f;
	storeFov = fovAngle;

	//light
	lightPos[0].Set(50.f, 120.f, 0.f);
	lightPos[1].Set(0.f, 1008.f, 0.f);

	//object
	InitTileMap();
	InitHero();
	InitObject();


	//minimap
	minimap = new MiniMap;
	minimap->SetBackground(MeshBuilder::GenerateMinimap("Minimap", Color(1, 1, 1), 1));
	minimap->GetBackground()->textureID[0] = LoadTGA("Image\\brick.tga");
	minimap->SetBorder(MeshBuilder::GenerateMinimapBorder("Minimap Border", Color(0, 1, 1)));
	minimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("Minimap avatar", Color(0, 1, 1)));
}

void ModelTerrain::InitObject()
{
	Object* obj_ptr;
	Mtx44 rotation, translation, scale;

	/* so that 'vector iterator wont be none incrementable' when passing in vector to add vehicle parts */
	elementObject.reserve(6);

	/* background */
	obj_ptr = new Object;
	obj_ptr->Set("background", Geometry::meshList[Geometry::GEO_AURORA_BOREALIS], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);	//rotate to face top by X
	obj_ptr->scaleObject(800, 600, 1);
	elementObject.push_back(obj_ptr);

	/** init **/
	for(std::vector<Object*>::iterator it = elementObject.begin(); it != elementObject.end(); ++it)
	{
			Object *go = (Object *)*it;
			go->Init();
	}
}

void ModelTerrain::InitTileMap()
{
	/* main tile map */
	m_Map = new Map();
	m_Map->Init(128, 25, 25);
	m_Map->LoadMap("Image//MapDesign.csv");

	/* background tile map 1 */
	m_backgroundSpeed_Percent = 0.7f;
	m_backgroundMap = new Map();
	m_backgroundMap->Init(64, 25, 25);
	m_backgroundMap->LoadMap("Image//MapBackground.csv");
}

void ModelTerrain::InitHero()
{
	/* constrains/Deadzone */
	constrain_PercentX = 0.1f;	//how much of screen you want to constrain
	middlePos.Set(800 / 2, 0);	//screen width / 2
	constrain_DistX = 800 * constrain_PercentX;
	offset.Set(0, 0);
	tileOffset_X = tileOffset_Y = 0;

	/* hero set */
	//hero.Init(Geometry::animation[0], 200, 400, 100, 100, m_Map);
}

Vector3 translateMe(0, 0, 0);
float scaleMe = 0.f;
void ModelTerrain::Update(double dt, bool* myKeys)
{
	Model::Update(dt, myKeys);

	/* fov */
	UpdateFOV(dt, myKeys);

	/* hero */
	//UpdateHero(dt, myKeys, m_view_width, m_view_height);

	/* tile */
	//UpdateTileMap(dt, m_view_width, m_view_height);

	for(std::vector<SpriteAnimation*>::iterator it = Geometry::animation.begin(); it != Geometry::animation.end(); ++it)
	{
		SpriteAnimation *go = (SpriteAnimation *)*it;
		go->Update(dt);
	}
}

void ModelTerrain::UpdateLight(double dt, bool* myKeys, Light* light)
{
}

void ModelTerrain::UpdateHero(double dt, bool* myKeys, float m_window_width, float m_window_height)
{
	//hero.Update(dt, myKeys, m_window_width, m_window_height, middlePos, constrain_DistX, offset, m_Map);
}

void ModelTerrain::UpdateTileMap(double dt, float m_window_width, float m_window_height)
{
	/**** TileMap ****/
	/* main tile */
	float tileLoc;	//see if tile is out of screen, if is, do not RENDER SAFE FCKING FPS
	for(int i = 0; i < m_Map->GetNumOfTiles_Height(); ++i)
	{
		for(int k = 0; k < m_Map->GetNumOfTiles_Width(); ++k)
		{
			tileLoc = static_cast<float>((k * m_Map->GetTileSize() - offset.x));

			if(m_Map->theScreenMap[i][k] != 0 && (tileLoc <= m_window_width && tileLoc + m_Map->GetTileSize() >= 0))
			{
				//mesh, enableLight, size X, size Y, pos X, pos Y
				//Render2DTile(Geometry::meshList[Geometry::GEO_TILEMAP], false, 1.0f, (k * model->m_Map->GetTileSize()) - model->offset.x, i * model->m_Map->GetTileSize(), model->m_Map->theScreenMap[i][k]);
			}
		}
	}
}

void ModelTerrain::Exit()
{
	//minimap
	delete minimap;

	if(m_Map)
	{
		delete m_Map;
		m_Map = NULL;
	}

	Model::Exit();
}