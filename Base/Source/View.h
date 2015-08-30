#ifndef VIEW_H
#define VIEW_H
#include "Model_Level.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

/**
 Everything in view class will be using Objects class for TRS and other info
 **/
class View
{
protected:
/********************** openGL *********************************/
	static GLFWwindow* m_window_view;
	Model* model;	//point to model_level/model_menu..etc of child classes

/********************** Window screen size *****************************/
	//dimension on computer screen
	static unsigned short m_console_width;
	static unsigned short m_console_height;

	static bool initAlready;

public:
/********************** Light *****************************/
	const static unsigned short m_total_lights = 2;

/********************* Uniform parameters ***********************/
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,

		/* material */
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		/* light */
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		/* texture */
		U_COLOR_TEXTURE_ENABLED,	//multitexture: texture enabled
		U_COLOR_TEXTURE_ENABLED_1,
		U_COLOR_TEXTURE,			//multitexture:	color enabled
		U_COLOR_TEXTURE_1,

		/* TEXT */
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		/* FOG */
		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,

		U_TOTAL,
	};

	enum MODE
	{
		TWO_D,
		THREE_D,
	};

	enum RENDER_TYPE
	{
		MENU,	//rendering game menu, instruction screens etc...
		GAME,	//rendering of in-game scene, levels....
	};

/********************** constructor/destructor *****************************/
	View();
	View(unsigned short console_width, unsigned short console_height, MODE mode);
	virtual ~View();

/********************** Core functions *****************************/
	virtual void SetModel(Model_Level* model_level) = 0;	//if not model_Level, set to NULL
	virtual void Init();
	void InitLight();
	void InitFontData();
	void InitProjection();

	virtual void Render(const float fps);
	virtual void Exit();

/**************** render ****************/

	/* Text */
	void RenderText(Mesh* mesh, std::string text, Color color);

	//render text with cutoff: if string longer than cutoff, go to next line
	void RenderTextOnScreenCutOff(Mesh* mesh, std::string text, Color color, float size, float x, float y, float z = 1.f, int cutOff = 32);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float z = 1.f);

	/* Render mesh */
	void RenderMesh(Mesh *mesh, bool enableLight);

	/* 2D */
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizex=1.0f, float sizey = 1.0f, float sizez = 1.0f, float x=0.0f, float y=0.0f, float z = 0.f, float angle = 0.f);
	void Render2DTile(Mesh *mesh, bool enableLight, float size, float x, float y, int tileType);

/********************** Console screen size *****************************/
	static unsigned short getConsoleHeight();
	static unsigned short getConsoleWidth();

/********************** openGL *********************************/
	static GLFWwindow* getWindow();
	RENDER_TYPE getRenderType();

protected:
/************* mode *****************/
	MODE mode;
	RENDER_TYPE render_type;

/************* matrix *****************/
	static MS modelStack;
	static MS viewStack;
	static MS projectionStack;

/************* lights *****************/
	static Light lights[m_total_lights];	//for model, use the lights provided in view

/********************** openGL *********************************/
	static unsigned m_vertexArrayID;
	static unsigned m_programID;
	static unsigned m_parameters[U_TOTAL];
	static float fps;

/********************** text **********************/
	static float FontData[256];
};

#endif