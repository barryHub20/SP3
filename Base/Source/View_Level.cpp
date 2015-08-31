#include "View_Level.h"
//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadHmap.h"
#include "SpriteAnimation.h"
#include "Controller.h"
#include "FontData.h"
#include <sstream>

/*****
!! RenderTileMap has a hardcoded stuff (screen size)
*****/

View_Level::View_Level()
{
}

View_Level::View_Level(unsigned short console_width, unsigned short console_height, MODE mode)
{
	this->render_type = View::GAME;
	this->m_console_width = console_width;
	this->m_console_height = console_height;
	this->mode = mode;
}

View_Level::~View_Level()
{
}

/********************** Core functions *****************************/
void View_Level::SetModel(Model_Level* model_level)
{
	/* Render a model_level level */
	if(model_level != NULL && render_type == View::GAME)
	{
		this->model_level = model_level;
		model = this->model_level;
	}
}

void View_Level::Init()
{
	if(!initAlready)
	{
		View::Init();
	}
}

void View_Level::Render(const float fps)
{
	View::Render(fps);

	switch (model_level->stateManager.GetState())
	{
		case StateManager::GAME:
		{
			RenderGame();
			break;
		}
		case StateManager::MAIN_MENU:
		{
			RenderMainMenu();
			break;
		}
		case StateManager::INSTRUCTION:
		{
			RenderInstruction();
			break;
		}
		case StateManager::TRANSITION:
		{
			RenderTransition();
			break;
		}
		default:
		{
			cout << "NO STATE" << endl;
		}
	}
}

void View_Level::RenderCollideBox()
{
	Vector3 pos, scale;

	if (model_level->stateManager.GetState() == model_level->stateManager.GAME)	// If GAME is current state
	{
		for(vector<GameObject*>::iterator it = model_level->getObject()->begin(); it != model_level->getObject()->end(); ++it)
		{
			
		}
	}
}

void View_Level::RenderHUD()
{
	//render UI
	/* UI List */
	for(vector<UI_Object*>::iterator it = model_level->getUIList()->begin(); it != model_level->getUIList()->end(); ++it)
	{
		UI_Object* o = (UI_Object*)*it;

		if(o->getActive())
		{
			modelStack.PushMatrix();
			modelStack.LoadMatrix( *(o->getTRS()) );
			RenderMeshIn2D(o->getMesh(), o->getLight(), o->getScale().x, o->getScale().y, o->getScale().z, o->getPosition().x, o->getPosition().y, o->getPosition().z, 0);
			modelStack.PopMatrix();
		}
	}

	//On screen text
	if(Geometry::meshList[Geometry::GEO_AR_CHRISTY] != NULL)
	{	
		int playerHealth;
		playerHealth = model_level->player->getHealth();
		int playerStamina;
		playerStamina = model_level->player->getStamina();
		
		RenderMeshIn2D(Geometry::meshList[Geometry::GEO_HEALTHBARBG], false, 50.f, 5.f, 1.f, 5.f, 114.f, 0.f, 0.f);
		RenderMeshIn2D(Geometry::meshList[Geometry::GEO_HEALTHBARCOLOR], false, 47.f * 0.01f * playerHealth, 1.5f, 1.f, 4.5f, 116.25f, 1.f, 0.f);
		RenderMeshIn2D(Geometry::meshList[Geometry::GEO_HEALTHBARMARKER], false, 1.f, 4.f, 2.f,  47.f * 0.01f * playerHealth + 3.75f, 115.f, 2.f, 0.f);
		
		RenderMeshIn2D(Geometry::meshList[Geometry::GEO_STAMINABARCOLOR], false, 47.f * 0.01f * playerStamina, 3.f, 1.f, 4.5f, 110.25f, 1.f, 0.f);
		RenderMeshIn2D(Geometry::meshList[Geometry::GEO_STAMINABARMARKER], false, 8.f, 8.f, 2.f, 47.f * 0.01f * playerStamina - 1.f, 109.f, 2.f, 0.f);

		//fps
		ostringstream ss;

		ss.str("");
		ss.precision(3);
		ss << "FPS: " << model_level->getFPS();
		if (model_level->getFPS() < 50)
		{
			//cout << "below 50" << endl;
		}
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(0.25f, 0.25f, 0.25f), 5, 15, 5);

		ss.str("");
		ss.precision(3);
		ss << "Pos: " << model_level->player->getPosition();
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 0, 0), 5, 25, 10);

		//Puzzle
		if (model_level->puzzleOpen == true)
		{
			Puzzle* tempPuzzle;
			tempPuzzle = model_level->puzzleManager->getCurrentPuzzle();
			if (tempPuzzle->getType() == Puzzle::WORD)
			{
				ss.str(tempPuzzle->getTextPuzzle());
				RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(0.25f, 0.25f, 0.25f), 5, 50, 25);
			}
			else
			{
				RenderMeshIn2D(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], false, 50, 50, 50, 500, 500, 1);
			}
		}
	}
}

void View_Level::RenderMainMenu()
{
	//render UI
	/* UI List */
	for(vector<UI_Object*>::iterator it = model_level->getUIList()->begin(); it != model_level->getUIList()->end(); ++it)
	{
		UI_Object* o = (UI_Object*)*it;

		if(o->getActive())
		{
			modelStack.PushMatrix();
			modelStack.LoadMatrix( *(o->getTRS()) );
			RenderMeshIn2D(o->getMesh(), o->getLight(), o->getScale().x, o->getScale().y, o->getScale().z, o->getPosition().x, o->getPosition().y, o->getPosition().z, 0);
			modelStack.PopMatrix();
		}
	}

	std::ostringstream ss;	//universal
	ss.precision(5);
	ss << "BREAK-IN!";
	RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 1, 1), 6, 80, 67);
	ss.str("");

	if (Controller::mouse_current_x < 618 && Controller::mouse_current_x > 243 && Controller::mouse_current_y < 631 && Controller::mouse_current_y > 600)
	{
		ss << "Click HERE to start!";
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 1, 0), 4, 80, 5);
		ss.str("");
	}
	else
	{
		ss << "Click HERE to start!";
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 0.5f, 0), 4, 80, 5);
		ss.str("");
	}

	if (Controller::mouse_current_x < 581 && Controller::mouse_current_x > 336 && Controller::mouse_current_y < 656 && Controller::mouse_current_y > 636)
	{
		ss << "( Instructions )";
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 1, 0), 3, 80, 3);
	}
	else
	{
		ss << "( Instructions )";
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 0.5f, 0), 3, 80, 3);
	}
}

void View_Level::RenderInstruction()
{
	std::ostringstream ss;	//universal
	ss.precision(5);
	ss << "WASD - Movement";
	RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 1, 1), 60, 100, 600);
	ss.str("");

	if (Controller::mouse_current_x < 280 && Controller::mouse_current_x > 60 && Controller::mouse_current_y < 540 && Controller::mouse_current_y > 520)
	{
		ss << "<--- Back";
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 1, 0), 40, 90, 150);
	}
	else
	{
		ss << "<--- Back";
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 0.5f, 0), 40, 90, 150);
	}
}

void View_Level::RenderGame()
{
	/* tile map */
	RenderTileMap();

	/* collide box */
	RenderCollideBox();

	/* HUD */
	RenderHUD();

	/* Inventory */
	RenderInventory(model_level->player->getInventory());
}

void View_Level::RenderTransition()
{
	std::ostringstream ss;	//universal
	ss.precision(5);
	ss << "----- LOADING -----";
	RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(1, 1, 1), 60, 240, 300);
	ss.str("");
}

void View_Level::RenderLight()
{
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(model_level->getLightPos(0).x, model_level->getLightPos(0).y, model_level->getLightPos(0).z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * model_level->getLightPos(0);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * model_level->getLightPos(0);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(model_level->getLightPos(1).x, model_level->getLightPos(1).y, model_level->getLightPos(1).z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * model_level->getLightPos(1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * model_level->getLightPos(1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	/** get position of lightball from model_level **/
	modelStack.PushMatrix();
	modelStack.Translate(model_level->getLightPos(0).x, model_level->getLightPos(0).y, model_level->getLightPos(0).z);
	modelStack.Scale(9, 9, 9);
	RenderMesh(Geometry::meshList[Geometry::GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(model_level->getLightPos(1).x, model_level->getLightPos(1).y, model_level->getLightPos(1).z);
	modelStack.Scale(9, 9, 9);
	RenderMesh(Geometry::meshList[Geometry::GEO_LIGHTBALL], false);
	modelStack.PopMatrix();
}

void View_Level::RenderObject()
{
	/* Renders all objects */
	if (Model_Level::stateManager.GetState() == Model_Level::stateManager.GAME)
	{
		for(vector<GameObject*>::iterator it = model_level->getObject()->begin(); it != model_level->getObject()->end(); ++it)
		{
			Object* o = (Object*)*it;

			if(o->getActive())
			{
				modelStack.PushMatrix();
				modelStack.LoadMatrix(*(o->getTRS()));
				RenderMesh(Geometry::meshList[Geometry::GEO_DEBUG_CUBE], false);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.LoadMatrix( *(o->getTRS()) );
				RenderMesh(o->getMesh(), o->getLight());
				modelStack.PopMatrix();
			}
		}
	}

	if (Model_Level::stateManager.GetState() == Model_Level::stateManager.MAIN_MENU || Model_Level::stateManager.GetState() == Model_Level::stateManager.INSTRUCTION || Model_Level::stateManager.GetState() == Model_Level::stateManager.TRANSITION)
	{
		RenderMeshIn2D(Geometry::meshList[Geometry::GEO_JINFLOOR],false,160,1,1,0,0);
	}
}

void View_Level::RenderTileMap()
{
	TileObject* tileObject = NULL;

	//Render main and other tile maps
	//Number of maps
	vector<Map*>* level_map = model_level->getLevelMap();

	float z = -4.f;
	//Render first map, usually floor
	for (int noMap = 0; noMap < level_map->size(); noMap++)
	{
		//Render floor
		if ((*level_map)[noMap]->getMapType() == Map::FLOORMAP)
		{
			if ((*level_map)[noMap]->getFloorMesh() == NULL) //If no quad, render floor tile
			{
				//Render tiles 
				float tileSize = (*level_map)[noMap]->GetTileSize(); //Get current tile size

				for (int i = 0; i < (*level_map)[noMap]->GetNumOfTiles_Height(); ++i)	//y
				{
					for (int k = 0; k < (*level_map)[noMap]->GetNumOfTiles_Width(); ++k)	//x
					{
						tileObject = (*level_map)[noMap]->getTileObject(k, i);

						if (tileObject->getTileType() == TileObject::NONE)	//skip rendering floors
							continue;

						modelStack.PushMatrix();
						modelStack.Translate(tileObject->getPosition().x, tileObject->getPosition().y, z);
						modelStack.Scale(tileObject->getScale().x, tileObject->getScale().y, 1);
						RenderTile(tileObject->getMesh(), false, tileObject->getTileNum());
						modelStack.PopMatrix();
					}
				}
			}
			else //Render floor quad
			{
				modelStack.PushMatrix();
				modelStack.Translate((*level_map)[noMap]->GetNumOfTiles_Width() * 32 * 0.5f, (*level_map)[noMap]->GetNumOfTiles_Height() * 32 * 0.5f, z);
				modelStack.Scale((*level_map)[noMap]->GetNumOfTiles_Width() * 32, (*level_map)[noMap]->GetNumOfTiles_Height() * 32, 1);
				RenderMesh((*level_map)[noMap]->getFloorMesh(), false);
				modelStack.PopMatrix();
			}
			z += 0.01f;
		}
	}

	//Render game objects
	RenderObject();

	//Render rest of the map
	for (int noMap = 0; noMap < level_map->size(); noMap++) 
	{
		if ((*level_map)[noMap]->getMapType() == Map::FLOORMAP) //If map is a floor, do not render again
		{
			continue; 
		}

		else if ((*level_map)[noMap]->getMapType() == Map::NOCOLLISIONMAP)
		{
			z += 2.f; //To create depth
		}
		
		for (int i = 0; i < (*level_map)[noMap]->GetNumOfTiles_Height(); ++i)	//y
		{
			for (int k = 0; k < (*level_map)[noMap]->GetNumOfTiles_Width(); ++k)	//x
			{
				tileObject = (*level_map)[noMap]->getTileObject(k, i);

				if (tileObject->getTileType() == TileObject::NONE)	//skip rendering floors
					continue;

				modelStack.PushMatrix();
				modelStack.Translate(tileObject->getPosition().x, tileObject->getPosition().y, z);
				modelStack.Scale(tileObject->getScale().x, tileObject->getScale().y, 1);
				RenderTile(tileObject->getMesh(), false, tileObject->getTileNum());
				modelStack.PopMatrix();
			}
		}

		if ((*level_map)[noMap]->getMapType() == Map::NOCOLLISIONMAP)
		{
			z -= 2.f;
		}
		z += 0.01f;
	}
}

void View_Level::RenderInventory(Inventory* inventory)
{
	int size = 0;
	string name = "";
	float startX = inventory->getStartX();
	float yPos = inventory->getYPos();
	Vector3 scale = inventory->getSlotScale();
	Mesh* mesh = NULL;
	ostringstream ss;
	
	for(int i = 0; i < inventory->MAX_SLOT; ++i)
	{
		/* render slot */
		if(i == inventory->getCurrentSlot())
		{
			RenderMeshIn2D(Geometry::meshList[Geometry::GEO_SLOT_SELECTED], false, scale.x, scale.y, 1,startX, yPos, 2.f, 0);
		}
		else
		{
			RenderMeshIn2D(Geometry::meshList[Geometry::GEO_SLOT_UNSELECTED], false, scale.x, scale.y, 1,startX, yPos, 2.f, 0);
		}

		/* render current size */
		size = inventory->currentSize(i);
		ss.str("");
		ss << size;
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(32.f / 255.f, 94.f / 255.f, 11.f / 255.f), scale.x * 0.2f, startX, yPos - scale.y * 0.35f, 2.1f);

		/* render name */
		name = inventory->currentItemName(i);
		ss.str("");
		ss << name;
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(166.f / 255.f, 23.f / 255.f, 23.f / 255.f), scale.x * 0.2f, startX, yPos + scale.y * 0.36f, 2.1f);

		/* render mesh */
		mesh = inventory->currentItemMesh(i);

		if(mesh != NULL)
		{
			RenderMeshIn2D(mesh, false, scale.x * 0.65f, scale.y * 0.65f, 1, startX, yPos, 2.1f, 1);
		}
		
		//set pos to next slot
		startX += inventory->getDistBtwSlot();
	}

	////test
	//ss.str("");
	//ss << "Hey I just met yyou and this is crazy so heres my number so call me maybe";
	//RenderTextOnScreenCutOff(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(32.f / 255.f, 94.f / 255.f, 11.f / 255.f), 5, 50, 50, 3.1f, 32);
}

void View_Level::RenderRearMap()
{
	
}

void View_Level::RenderTile(Mesh* mesh, bool enableLight, int tileNum)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	/* week 6 fog */
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if(enableLight && model_level->getbLightEnabled())
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);

		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}


	for(int i = 0; i < 2; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
	}

	mesh->Render((tileNum - 1) * 6, 6);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void View_Level::Exit()
{
	// Cleanup VBO
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);


	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window_view);
	//Finalize and clean up GLFW
	glfwTerminate();
}