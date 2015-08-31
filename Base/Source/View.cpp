#include "View.h"
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

//openGL
GLFWwindow* View:: m_window_view;
unsigned short View::m_console_width = 0;
unsigned short View::m_console_height = 0;
/************* matrix *****************/
MS View::modelStack;
MS View::viewStack;
MS View::projectionStack;

/************* lights *****************/
Light View::lights[m_total_lights];	//for model, use the lights provided in view

/********************** openGL *********************************/
unsigned View::m_vertexArrayID;
unsigned View::m_programID;
unsigned View::m_parameters[U_TOTAL];
float View::fps;

/********************** text **********************/
float View::FontData[256];
bool View::initAlready = false;

/** Utilities: for public use **/
std::ostringstream ss;	//universal

//Define an error callback
static void error_callback_view(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback_view(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback_view(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

View::View()
{
}

View::View(unsigned short console_width, unsigned short console_height, MODE mode) : mode(mode)
{
	m_console_width = console_width;
	m_console_height = console_height;
}

View::~View()
{
}

/********************** Window screen size *****************************/
unsigned short View::getConsoleHeight(){return m_console_height;}
unsigned short View::getConsoleWidth(){return m_console_width;}

/********************** Core functions *****************************/
void View::Init()
{

	initAlready = true;

	//Set the error callback
	glfwSetErrorCallback(error_callback_view);

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
	m_window_view = glfwCreateWindow(m_console_width, m_console_height, "LALALALALLA", NULL, NULL);

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
	glfwSetWindowSizeCallback(m_window_view, resize_callback_view);

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
//	glClearColor(72.f / 255.f, 240.f / 255.f, 125.f / 255.f, 0.0f);
	glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
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
	m_programID = LoadShaders( "Shader//fog.vertexshader", "Shader//fog.fragmentshader" );

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
	bool fogEnable = false;

	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &color.r);
	glUniform1f(m_parameters[U_FOG_START], fogStart);
	glUniform1f(m_parameters[U_FOG_END], fogEnd);
	glUniform1f(m_parameters[U_FOG_DENSITY], fogDensity);
	glUniform1f(m_parameters[U_FOG_ENABLED], fogEnable);
	glUniform1i(m_parameters[U_FOG_TYPE], fogType);
	glUniform1i(m_parameters[U_FOG_ENABLED], 1);
}

void View::InitProjection()
{
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 unit
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, m_console_width * (1 / m_console_height), 1.f, 12000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
}
void View::InitFontData()
{
	getFontData("Image//ar_christy.csv", FontData);
}

void View::InitLight()
{
	/** SET POS IN MODEL **/
	lights[0].type = Light::LIGHT_DIRECTIONAL;
	//lights[0].position.Set(0.f, 1008.f, 0.f);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1.f;
	//lights[0].kC = 1.f;
	//lights[0].kL = 0.01f;
	//lights[0].kQ = 0.001f;
	//lights[0].cosCutoff = cos(Math::DegreeToRadian(45)); //cut off the lights spot after this degree
	//lights[0].cosInner = cos(Math::DegreeToRadian(30));
	//lights[0].exponent = 3.f;
	////lights[0].spotDirection.Set(0.f, -1.f, 0.f);

	lights[1].type = Light::LIGHT_POINT;
	//lights[1].position.Set(0.f, 1008.f, 0.f);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 100.f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45)); //cut off the lights spot after this degree
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, -1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], m_total_lights);	//2 lights

	/* pass in uniform parameters for lightsource */
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);


	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);
}

void View::Render(const float fps)
{
	this->fps = fps;
	/* initialize render */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;

	/** 3D **/
	if(mode == THREE_D)
		perspective.SetToPerspective(45, m_console_width * (1.f / m_console_height), 0.1f, 14000.0f);
	
	/** 2D **/
	else
		perspective.SetToOrtho(0, model->getViewWidth(), 0, model->getViewHeight(), -10.f, 10.f);

	projectionStack.LoadMatrix(perspective);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		model->getCamera()->position.x, model->getCamera()->position.y, model->getCamera()->position.z,
		model->getCamera()->target.x,  model->getCamera()->target.y,  model->getCamera()->target.z,
		model->getCamera()->up.x, model->getCamera()->up.y, model->getCamera()->up.z
		);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();


void View::Exit()
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
GLFWwindow* View::getWindow()
{
	return m_window_view;
}

/**************** render ****************/
void View::RenderText(Mesh* mesh, std::string text, Color color)
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

float lengthOffset = 0;
float zOffset = 0;
float textLength = 0;
float textXLength = 0;
void View::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float z)
{
	if(!mesh || mesh->textureID[0] <= 0 || text.length() == 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, model->get2DViewWidth(), 0, model->get2DViewHeight(), -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, z);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	/* get total scale.x of string */
	textLength = text.length();
	textXLength = 0.f;
	for(unsigned i = 0; i < textLength; ++i)
	{
		textXLength += FontData[text[i]];
	}

	lengthOffset = (textXLength * -0.5f) + (FontData[text[0]] * 0.5f);	//make sure is start from center
	for(unsigned i = 0; i < textLength; ++i)
	{
		Mtx44 characterSpacing;

		characterSpacing.SetToTranslation(lengthOffset, 0.f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		lengthOffset += FontData[text[i]];

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	lengthOffset = 0.f;	//reset length
	textXLength = 0.f;

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void View::RenderTextOnScreenCutOff(Mesh* mesh, std::string text, Color color, float size, float x, float y, float z, int cutOff)
{
	if(!mesh || mesh->textureID[0] <= 0 || text.length() == 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, model->get2DViewWidth(), 0, model->get2DViewHeight(), -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, z);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	/* start counting */
	textLength = text.length();
	lengthOffset = 0.f;
	float yp = 0.f;
	for(unsigned i = 0; i < textLength; ++i)
	{
		if(i >= cutOff)	//if exceedd cutoff, go to next line
		{
			cutOff *= 2;
			yp -= 1.f;
			lengthOffset = 0.f;
		}

		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(lengthOffset, yp, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		lengthOffset += FontData[text[i]];

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	lengthOffset = 0.f;	//reset length
	textXLength = 0.f;

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void View::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	/* week 6 fog */
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if(enableLight && model->getbLightEnabled())
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

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void View::RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizex, float sizey, float sizez, float x, float y, float z, float angle)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, model->get2DViewWidth(), 0, model->get2DViewHeight(), -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, z);
	modelStack.Rotate(angle, 0, 0, 1);
	modelStack.Scale(sizex, sizey, sizez);

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

void View::Render2DTile(Mesh *mesh, bool enableLight, float size, float x, float y, int tileType)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_console_width, 0, m_console_height, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, -2);
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

View::RENDER_TYPE View::getRenderType()
{
	return render_type;
}

//Model_Level* View::getModel(int index)
//{
//	return modelList[index];
//}
//
//int View::modelListSize()
//{
//	return modelList.size();
//}
//
//void View::SetNewModel(int index)
//{
//	this->model = modelList[index];
//}
