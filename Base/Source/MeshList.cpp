#include "MeshList.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Utility.h"
#include "LoadOBJ.h"
#include "LoadHmap.h"
#include "LoadTGA.h"

Mesh* Geometry::meshList[NUM_GEOMETRY];
vector<SpriteAnimation*> Geometry::animation;
vector<unsigned char> Geometry::m_heightMap;

	/* core */
void Geometry::Init()
{
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_DEBUG_CUBE] = MeshBuilder::GenerateDebugCube("Cube", Color(1, 0, 0), 1);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_AR_CHRISTY] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_AR_CHRISTY]->textureID[0] = LoadTGA("Image//ar_christy.tga");
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 1.f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", Color(179.f / 255.f, 68.f / 255.f, 9.f / 255.f), 1);

	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 1.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

	/** world object **/
	meshList[GEO_OBJECT] = MeshBuilder::GenerateQuad("object", Color(1, 1, 1));
	meshList[GEO_PAVEMENT] = MeshBuilder::GenerateOBJ("pavement", "OBJ//pavement2.obj");//MeshBuilder::GenerateCube("cube", 1);
	meshList[GEO_PAVEMENT]->textureID[0] = LoadTGA("Image//pavement.tga");


	/** UI **/
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("GEO_CROSSHAIR", Color(1, 1, 1), 1.f);
	meshList[GEO_CROSSHAIR]->textureID[0] = LoadTGA("Image//crosshair.tga");

	/** skybox **/
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID[0] = LoadTGA("Image//sunset_left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID[0] = LoadTGA("Image//sunset_right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID[0] = LoadTGA("Image//sunset_top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID[0] = LoadTGA("Image//sunset_bottom.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID[0] = LoadTGA("Image//sunset_front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);

	/** terrain **/
	meshList[GEO_BACK]->textureID[0] = LoadTGA("Image//sunset_back.tga");

	meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1, 1, 1), 128, 600.0f, 6000.0f, 6.0f, 6.0f);
	meshList[GEO_SKYPLANE]->textureID[0] = LoadTGA("Image//night_sky6.tga"); 

	meshList[GEO_TERRAIN] = meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//valley.raw", m_heightMap);  
	meshList[GEO_TERRAIN]->textureID[0] = LoadTGA("Image//moss1.tga"); 
	meshList[GEO_TERRAIN]->textureID[1] = LoadTGA("Image//brick.tga"); 

	meshList[GEO_SNOWFLAKE] = MeshBuilder::GenerateOBJ("snow flake", "OBJ//snow_flake.obj");
	meshList[GEO_SNOWFLAKE]->textureID[0] = LoadTGA("Image//snow_flake.tga");

	
	meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJ("OBJ1", "OBJ//building-2.obj");//MeshBuilder::GenerateCube("cube", 1);
	meshList[GEO_BUILDING]->textureID[0] = LoadTGA("Image//building-1.tga");

	meshList[GEO_SIGNBOARD] = MeshBuilder::GenerateOBJ("signboard", "OBJ//signboard.obj");//MeshBuilder::GenerateCube("cube", 1);
	meshList[GEO_SIGNBOARD]->textureID[0] = LoadTGA("Image//signboard.tga");
	meshList[GEO_LAMPPOST] = MeshBuilder::GenerateOBJ("signboard", "OBJ//lamppost.obj");//MeshBuilder::GenerateCube("cube", 1);
	meshList[GEO_LAMPPOST]->textureID[0] = LoadTGA("Image//signboard.tga");

	/* police car */
	meshList[GEO_CAR_MAIN_PART] = MeshBuilder::GenerateOBJ("car main part", "OBJ//car_main_part.obj");
	meshList[GEO_CAR_MAIN_PART]->textureID[0] = LoadTGA("Image//car_main_part.tga");
	meshList[GEO_CAR_GLASS] = MeshBuilder::GenerateOBJ("car glass", "OBJ//car_glass.obj");
	meshList[GEO_CAR_GLASS]->textureID[0] = LoadTGA("Image//car_glass.tga");
	meshList[GEO_CAR_SIREN] = MeshBuilder::GenerateOBJ("car siren", "OBJ//siren.obj");
	meshList[GEO_CAR_SIREN]->textureID[0] = LoadTGA("Image//siren.tga");

	/* HUD */
	meshList[GEO_HEALTHBARBG] = MeshBuilder::Generate2DMesh("healthbar background", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_HEALTHBARBG]->textureID[0] = LoadTGA("Image//HUD//healthbar-bg.tga");

	meshList[GEO_HEALTHBARCOLOR] = MeshBuilder::Generate2DMesh("healthbar color", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_HEALTHBARCOLOR]->textureID[0] = LoadTGA("Image//HUD//healthbar-color.tga");

	meshList[GEO_HEALTHBARMARKER] = MeshBuilder::Generate2DMesh("healthbar marker", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_HEALTHBARMARKER]->textureID[0] = LoadTGA("Image//HUD//healthbar-marker.tga");

	meshList[GEO_STAMINABARCOLOR] = MeshBuilder::Generate2DMesh("staminabar color", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_STAMINABARCOLOR]->textureID[0] = LoadTGA("Image//HUD//staminabar-color.tga");

	meshList[GEO_STAMINABARMARKER] = MeshBuilder::Generate2DMesh("staminabar marker", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_STAMINABARMARKER]->textureID[0] = LoadTGA("Image//HUD//staminabar-marker.tga");

	/* Tile */
	meshList[GEO_TILEMAP] = MeshBuilder::GenerateTileMap("Tile Map", Color(1, 1, 1), 1.f, 1.f, 8, 8);	//set the width/heignt as 1 (can scale later)
	meshList[GEO_TILEMAP]->textureID[0] = LoadTGA("Image//tile2_ground.tga");
	
	meshList[GEO_DUNGEONTILE] = MeshBuilder::GenerateTileMap("Dungeon tile map", Color(1, 1, 1), 1.f, 1.f, 4, 4);
	meshList[GEO_DUNGEONTILE]->textureID[0] = LoadTGA("Image//Sprites//dungeonwall.tga");

	meshList[GEO_TILESET1] = MeshBuilder::GenerateTileMap("Tile set 1", Color(1, 1, 1), 1.f, 1.f, 16, 16);
	meshList[GEO_TILESET1]->textureID[0] = LoadTGA("Image//Sprites//tileset1.tga");

	meshList[GEO_TEMPFLOOR] = MeshBuilder::GenerateTileMap("Temp floor", Color(1, 1, 1), 1.f, 1.f, 16, 16);
	meshList[GEO_TEMPFLOOR]->textureID[0] = LoadTGA("Image//Sprites//floor.tga");

	meshList[GEO_JINFLOOR] = MeshBuilder::GenerateQuad("Jin", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_JINFLOOR]->textureID[0] = LoadTGA("Image//Map//jin.tga");

	/* animation */
	meshList[GEO_GUARD] = MeshBuilder::GenerateSpriteAnimation("guard", 21, 13, 1.f);
	meshList[GEO_GUARD]->textureID[0] = LoadTGA("Image//Sprites//guard.tga");

	meshList[GEO_DOOR] = MeshBuilder::GenerateQuad("Door", Color(1, 1, 1), 1.f);
	meshList[GEO_DOOR]->textureID[0] = LoadTGA("Image//door.tga");

	meshList[GEO_ISTRIGGER] = MeshBuilder::GenerateQuad("is triggered", Color(1, 1, 1), 1.f);
	meshList[GEO_ISTRIGGER]->textureID[0] = LoadTGA("Image//isTriggered.tga");
	
	meshList[GEO_NOTTRIGGER] = MeshBuilder::GenerateQuad("not triggered", Color(1, 1, 1), 1.f);
	meshList[GEO_NOTTRIGGER]->textureID[0] = LoadTGA("Image//notTriggered.tga");

	/* Dungeon Stuffs */
	meshList[GEO_KEY] = MeshBuilder::GenerateQuad("Key", Color(1, 1, 1), 1.f);
	meshList[GEO_KEY]->textureID[0] = LoadTGA("Image//key.tga");

	meshList[GEO_HPOTION] = MeshBuilder::GenerateQuad("Health Potion", Color(1, 1, 1), 1.f);
	meshList[GEO_HPOTION]->textureID[0] = LoadTGA("Image//hPotion.tga");

	meshList[GEO_SPOTION] = MeshBuilder::GenerateQuad("Stamina Potion", Color(1, 1, 1), 1.f);
	meshList[GEO_SPOTION]->textureID[0] = LoadTGA("Image//sPotion.tga");

	meshList[GEO_ATKPOTION] = MeshBuilder::GenerateQuad("Attack Potion", Color(1, 1, 1), 1.f);
	meshList[GEO_ATKPOTION]->textureID[0] = LoadTGA("Image//atkPotion.tga");
}

void Geometry::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}

	/*while(animation.size() > 0)
	{
		SpriteAnimation *go = animation.back();
		delete go;
		animation.pop_back();
	}*/
}