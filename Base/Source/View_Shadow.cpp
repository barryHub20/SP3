#include "View_Shadow.h"
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
!! cullFace rmb uncomment at renderGPass and renderMain
*****/

//openGL
GLFWwindow*View_Shadow:: m_window_view;

//Define an error callback
static void error_callback_view_Shadow(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback_view_Shadow(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback_view_Shadow(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

View_Shadow::View_Shadow() : model(NULL)
{
}

View_Shadow::View_Shadow(Model* model) : model(model)
{
}

View_Shadow::~View_Shadow()
{
}

/********************** Window screen size *****************************/
int View_Shadow::getWindowHeight(){return m_window_width;}
int View_Shadow::getWindowWidth(){return m_window_height;}

/********************** Core functions *****************************/
void View_Shadow::Init(MODE mode)
{
	//mode
	this->mode = mode;

	//Set the error callback
	glfwSetErrorCallback(error_callback_view_Shadow);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window_view = glfwCreateWindow(m_window_width, m_window_height, "DM2231_Framework", NULL, NULL);

	//If the window couldn't be created
	if (!m_window_view)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window_view);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window_view, resize_callback_view_Shadow);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	/************* openGL stuff ****************/
	// Black background
	glClearColor(1.f, 1.f, 1.f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);


	/********************************* shader and stuff *********************************/
	//m_programID = LoadShaders( "Shader//fog.vertexshader", "Shader//fog.fragmentshader" );
	/* shadow */
	m_gPassShaderID = LoadShaders( "Shader//GPass.vertexshader", "Shader//GPass.fragmentshader" );
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	
	m_programID = LoadShaders( "Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader" );
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");


	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED_1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	//fog
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	// Use our shader
	glUseProgram(m_programID);

	InitFontData();
	InitLight();
	InitProjection();

	/**************************** fog ****************************/
	Color color = Color(196.f / 255.f, 196.f / 255.f, 196.f / 255.f);
	int fogStart = 200;
	int fogEnd = 1000;
	float fogDensity = 0.0005f;
	int fogType = 2;
	bool fogEnable = true;

	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &color.r);
	glUniform1f(m_parameters[U_FOG_START], fogStart);
	glUniform1f(m_parameters[U_FOG_END], fogEnd);
	glUniform1f(m_parameters[U_FOG_DENSITY], fogDensity);
	glUniform1f(m_parameters[U_FOG_ENABLED], fogEnable);
	glUniform1i(m_parameters[U_FOG_TYPE], fogType);

	//glUniform1i(m_parameters[U_FOG_ENABLED], 1);
}

void View_Shadow::InitProjection()
{
	// Projection matrix : 45� Field of View_Shadow, 4:3 ratio, display range : 0.1 unit <-> 1000 unit
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 1.f, 12000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
}
void View_Shadow::InitFontData()
{
	getFontData("Image//ar_christy.csv", FontData);
}

void View_Shadow::InitLight()
{
	/** SET POS IN MODEL **/
	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0.f, 1008.f, 0.f);
	lights[0].color.Set(227.f / 255.f, 121.f / 255.f, 16.f / 255.f);
	lights[0].power = 2.f;
	//lights[0].kC = 1.f;
	//lights[0].kL = 0.01f;
	//lights[0].kQ = 0.001f;
	//lights[0].cosCutoff = cos(Math::DegreeToRadian(65)); //cut off the lights spot after this degree
	//lights[0].cosInner = cos(Math::DegreeToRadian(40));
	//lights[0].exponent = 3.f;
	//lights[0].spotDirection.Set(0.f, -1.f, 0.f);

	lights[1].type = Light::LIGHT_POINT;
	//lights[1].position.Set(0.f, 1008.f, 0.f);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 10.f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45)); //cut off the lights spot after this degree
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, -1.f, 0.f);

	/* shadow: m_lightDepthFBO */
	m_lightDepthFBO.Init(1024, 1024);

	glUniform1i(m_parameters[U_NUMLIGHTS], m_total_lights);	//2 lights

	/* pass in uniform parameters for lightsource */
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	//glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	//glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	//glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	//glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	//glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	//glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);


	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);
}

void View_Shadow::Render(const float fps)
{
	this->fps = fps;
	/* initialize render */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;

	/** 3D **/
	//Projection matrix : 45� Field of View_Shadow, 4:3 ratio, display range : 0.1 unit <-> 1000 unit
	if(mode == THREE_D)
		perspective.SetToPerspective(model->getFOV(), m_window_width * (1.f / m_window_height), 0.1f, 14000.0f);

	/** 2D **/
	else
		perspective.SetToOrtho(-m_window_width_2D, m_window_width_2D, -m_window_height_2D, m_window_height_2D, -10, 10);

	projectionStack.LoadMatrix(perspective);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		model->getCamera()->position.x, model->getCamera()->position.y, model->getCamera()->position.z,
		model->getCamera()->target.x,  model->getCamera()->target.y,  model->getCamera()->target.z,
		model->getCamera()->up.x, model->getCamera()->up.y, model->getCamera()->up.z
		);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	/* axes */
	modelStack.PushMatrix();
	RenderMesh(Geometry::meshList[Geometry::GEO_AXES], false);
	modelStack.PopMatrix();


	if(mode == THREE_D)
		Render3D();
	else
		Render2D();


	/* HUD */
	RenderHUD();
}

void View_Shadow::Render3D()
{
	/* light */

	RenderPassGPass();

	RenderPassMain();


	/* collide box */
	//RenderCollideBox();
}

void View_Shadow::Render2D()
{
	/* test object */
	RenderObject();
}

void View_Shadow::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	//These matrices should change when light position or
	//direction changes
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-1500, 1500, -100, 1000, -1500, 1500);
	else
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);

	Vector3 lightPos(model->getLightPos(0).x, model->getLightPos(0).y, model->getLightPos(0).z);


	m_lightDepthView.SetToLookAt(model->getLightPos(0).x, model->getLightPos(0).y, model->getLightPos(0).z, 0, 0, 0, 0, 1, 0);

	RenderObject();
}
	
void View_Shadow::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_window_width, m_window_height);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	//old stuff
	/* initialize render */
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	// Projection matrix : 45� Field of View_Shadow, 4:3 ratio, display range : 0.1 unit <-> 1000 unit
	perspective.SetToPerspective(model->getFOV(), m_window_width * (1.f / m_window_height), 0.1f, 14000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		model->getCamera()->position.x, model->getCamera()->position.y, model->getCamera()->position.z,
						model->getCamera()->target.x, model->getCamera()->target.y, model->getCamera()->target.z,
						model->getCamera()->up.x, model->getCamera()->up.y, model->getCamera()->up.z
					);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	/* collide box */
	RenderCollideBox();

	modelStack.PushMatrix();
	RenderMesh(Geometry::meshList[Geometry::GEO_AXES], false);

	RenderObject();
	RenderLight();
}

void View_Shadow::RenderCollideBox()
{
}

void View_Shadow::RenderHUD()
{
	//On screen text
	if(Geometry::meshList[Geometry::GEO_AR_CHRISTY] != NULL)
	{
		std::ostringstream ss;	//universal
		ss.precision(5);
		ss << "FPS: " << fps;
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(0, 1, 0), 3, 5, 6);

		ss.str("");
		ss << "Pos: " << model->getCamera()->position;
		RenderTextOnScreen(Geometry::meshList[Geometry::GEO_AR_CHRISTY], ss.str(), Color(0, 1, 0), 3, 5, 9);
	}

	/** crosshair **/
	if(Geometry::meshList[Geometry::GEO_CROSSHAIR] != NULL)
	{
		/** crosshair **/
		RenderMeshIn2D(Geometry::meshList[Geometry::GEO_CROSSHAIR], false, 19);
	}
}

void View_Shadow::RenderLight()
{
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(model->getLightPos(0).x, model->getLightPos(0).y, model->getLightPos(0).z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * model->getLightPos(0);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * model->getLightPos(0);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(model->getLightPos(1).x, model->getLightPos(1).y, model->getLightPos(1).z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * model->getLightPos(1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * model->getLightPos(1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	/** get position of lightball from model **/
	modelStack.PushMatrix();
	modelStack.Translate(model->getLightPos(0).x, model->getLightPos(0).y, model->getLightPos(0).z);
	modelStack.Scale(9, 9, 9);
	RenderMesh(Geometry::meshList[Geometry::GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(model->getLightPos(1).x, model->getLightPos(1).y, model->getLightPos(1).z);
	modelStack.Scale(9, 9, 9);
	RenderMesh(Geometry::meshList[Geometry::GEO_LIGHTBALL], false);
	modelStack.PopMatrix();
}

void View_Shadow::RenderObject()
{
	if(mode == THREE_D)	//3D
	{
		/** Render for shadow **/
		if(m_renderPass == RENDER_PASS_PRE)
		{
			for(vector<Object*>::iterator it = model->getObject()->begin(); it != model->getObject()->end(); ++it)
			{
				Object* o = (Object*)*it;

				if(!o->getShadow())	//do not render if got NO Shadow
					continue;

				/* if got shadow: render it once in pre */
				modelStack.PushMatrix();
				modelStack.LoadMatrix( *(o->getTRS()) );
				RenderMesh(o->getMesh(), o->getLight());
				modelStack.PopMatrix();
			}
		}

		/** normal render **/
		else
		{
			for(vector<Object*>::iterator it = model->getObject()->begin(); it != model->getObject()->end(); ++it)
			{
				Object* o = (Object*)*it;
				modelStack.PushMatrix();
				modelStack.LoadMatrix( *(o->getTRS()) );
				RenderMesh(o->getMesh(), o->getLight());
				modelStack.PopMatrix();
			}
		}
	}
	else	//2D
	{
		//hero/characters
		//Render2DMesh(Geometry::meshList[Geometry::GEO_HERO], false, model->hero.scale.x, model->hero.scale.y, model->hero.pos.x, model->hero.pos.y);

		//tile map
		RenderTileMap();

		for(vector<Object*>::iterator it = model->getObject()->begin(); it != model->getObject()->end(); ++it)
		{
			Object* o = (Object*)*it;
			Render2DMesh(o->getMesh(), o->getLight(), o->getScale().x, o->getScale().y, o->getPosition().x, o->getPosition().y);
		}
	}
}

void View_Shadow::RenderTileMap()
{
	/* main tile */
	float tileLoc;	//see if tile is out of screen, if is, do not RENDER SAFE FCKING FPS
	for(int i = 0; i < model->m_Map->GetNumOfTiles_Height(); ++i)
	{
		for(int k = 0; k < model->m_Map->GetNumOfTiles_Width(); ++k)
		{
			tileLoc = static_cast<float>((k * model->m_Map->GetTileSize() - model->offset.x));

			if(model->m_Map->theScreenMap[i][k] != 0 && (tileLoc <= m_window_width && tileLoc + model->m_Map->GetTileSize() >= 0))
			{
				//mesh, enableLight, size X, size Y, pos X, pos Y
				Render2DTile(Geometry::meshList[Geometry::GEO_TILEMAP], false, 1.0f, (k * model->m_Map->GetTileSize()) - model->offset.x, i * model->m_Map->GetTileSize(), model->m_Map->theScreenMap[i][k]);
			}
		}
	}


	/* background layer 1: parallex scrolling */
	for(int i = 0; i < model->m_backgroundMap->GetNumOfTiles_Height(); ++i)
	{
		for(int k = 0; k < model->m_backgroundMap->GetNumOfTiles_Width(); ++k)
		{
			tileLoc = static_cast<float>((k * (model->m_backgroundMap->GetTileSize()) - (model->offset.x * model->m_backgroundSpeed_Percent)));

			if(model->m_backgroundMap->theScreenMap[i][k] != 0 && (tileLoc <= m_window_width && tileLoc + model->m_backgroundMap->GetTileSize() >= 0))
			{
				//mesh, enableLight, size X, size Y, pos X, pos Y
				Render2DTile(Geometry::meshList[Geometry::GEO_TILEMAP], false, 1.0f, (k * model->m_backgroundMap->GetTileSize()) - (int)(model->offset.x * model->m_backgroundSpeed_Percent), i * model->m_backgroundMap->GetTileSize(), model->m_backgroundMap->theScreenMap[i][k]);
			}
		}
	}
}

void View_Shadow::Exit()
{
	// Cleanup VBO
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);


	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window_view);
	//Finalize and clean up GLFW
	glfwTerminate();
}

/********************** openGL *********************************/
GLFWwindow* View_Shadow::getWindow()
{
	return m_window_view;
}

/**************** render ****************/
void View_Shadow::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}

float lengthOffset_Shadow = 0;
float zOffset_Shadow = 0;
void View_Shadow::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID[0] <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_window_width_2D, 0, m_window_height_2D, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;

		characterSpacing.SetToTranslation(lengthOffset_Shadow, 1.f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		lengthOffset_Shadow += FontData[text[i]];

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	lengthOffset_Shadow = 0.f;	//reset length
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void View_Shadow::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
		if(m_renderPass == RENDER_PASS_PRE)
		{
			Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
			
			glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1,
			GL_FALSE, &lightDepthMVP.a[0]);
			mesh->Render();
			return;
		}
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
			modelView = viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE,
			&modelView.a[0]);
			if(enableLight)
			{
				glUniform1i(m_parameters[U_LIGHTENABLED], 1);
					modelView_inverse_transpose =
					modelView.GetInverse().GetTranspose();
					glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE],
					1, GL_FALSE, &modelView_inverse_transpose.a[0]);
					
					Mtx44 lightDepthMVP = m_lightDepthProj *
					m_lightDepthView * modelStack.Top();
					
					glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1,
					GL_FALSE, &lightDepthMVP.a[0]);
					
					//load material
					glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1,
					&mesh->material.kAmbient.r);
					glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1,
					&mesh->material.kDiffuse.r);
					glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1,
					&mesh->material.kSpecular.r);
					glUniform1f(m_parameters[U_MATERIAL_SHININESS],
					mesh->material.kShininess);
			}
			else
			{
				glUniform1i(m_parameters[U_LIGHTENABLED], 0);
			}
			for(int i = 0; i < 2; ++i)	//2 texture
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
		mesh->Render();
}

void View_Shadow::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y, float z, float angle)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, z);
	modelStack.Rotate(angle, 0, 0, 1);
	modelStack.Scale(size, size, size);

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]);
	if(mesh->textureID[0] > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
			1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
			0);
	}
	mesh->Render();
	if(mesh->textureID[0] > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void View_Shadow::Render3DonScreen(Mesh* mesh, Camera3* cam, bool enableLight, Vector3 scale)
{
	//modelStack.LoadMatrix(*(model->testMe.getTRS()));
	/* change FPSCamera to Camera* : can load many other types of camera */
	if(mesh != NULL)
	{
		float offsetFromCam = 3.1f;

		Vector3 view, right;

		view = (cam->target - cam->position).Normalized();
		right = view.Cross(cam->up);
		right.y = 0;
		right.Normalize();
		float bitch = Math::RadianToDegree(asin(view.y)); 
		float angle = Math::RadianToDegree(atan2(view.x, view.z));

		glDisable(GL_DEPTH_TEST);
		modelStack.PushMatrix();
		modelStack.Translate(cam->target.x, cam->target.y, cam->target.z);
		modelStack.Rotate(bitch, right.x, right.y, right.z);//pitch rotation
		modelStack.Rotate(-angle, 0, 1, 0);	//yaw rotation

		modelStack.Translate(-offsetFromCam, 0, offsetFromCam);	//offset for infront of camera
		modelStack.Scale(scale.x, scale.y, scale.z);

		RenderMesh(mesh, enableLight);
		modelStack.PopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
}

void View_Shadow::Render2DMesh(Mesh *mesh, bool enableLight, float sizeX, float sizeY, float x, float y)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizeX, sizeY, 1);

	/*if (rotate)
		modelStack.Rotate(rotateAngle, 0, 0, 1);*/

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
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

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void View_Shadow::Render2DTile(Mesh *mesh, bool enableLight, float size, float x, float y, int tileType)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, 1);

	/*if (rotate)
		modelStack.Rotate(rotateAngle, 0, 0, 1);*/

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
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

	mesh->Render((tileType - 1) * 6, 6);

	glBindTexture(GL_TEXTURE_2D, 0);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}