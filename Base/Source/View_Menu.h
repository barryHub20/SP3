#ifndef VIEW_MENU_H
#define VIEW_MENU_H
#include "View.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

/**
 For rendering of UI Menu
 **/
class View_Menu : public View
{
private:
/********************** model ptr **********************/
	Model_Level* model; //the current model

/********************** Window screen size *****************************/
	//dimension on computer screen
	static unsigned short m_console_width;
	static unsigned short m_console_height;

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

/********************** constructor/destructor *****************************/
	View_Menu();
	View_Menu(unsigned short console_width, unsigned short console_height, MODE mode);
	~View_Menu();

/********************** Core functions *****************************/
	void Init();
	void InitLight();
	void InitFontData();
	void InitProjection();
	void Render(const float fps, Model_Level* model);
	void Exit();

	void SetNewModel(int index);	//if your model is an array

/**************** render ****************/
	void RenderLight();
	void RenderObject();
	void RenderCollideBox();
	void RenderInventory(Inventory* inventory);
	void RenderHUD();
	void RenderMainMenu();
	void RenderInstruction();
	void RenderGame();
	void RenderTransition();

	/* If theres TileMap */
	void RenderTileMap();
	void RenderRearMap();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizex=1.0f, float sizey = 1.0f, float sizez = 1.0f, float x=0.0f, float y=0.0f, float z = 0.f, float angle = 0.f);
	void RenderMeshInLines(Mesh* mesh, const Vector3& position, const Vector3& scale);
	void Render2DTile(Mesh *mesh, bool enableLight, float size, float x, float y, int tileType);
	void RenderTile(Mesh* mesh, bool enableLight, int tileNum);

/********************** Console screen size *****************************/
	static unsigned short getConsoleHeight();
	static unsigned short getConsoleWidth();

/********************** openGL *********************************/
	static GLFWwindow* getWindow();

private:
/************* mode *****************/
	MODE mode;

/************* matrix *****************/
	MS modelStack;
	MS viewStack;
	MS projectionStack;

/************* lights *****************/
	Light lights[m_total_lights];	//for model, use the lights provided in view

/********************** openGL *********************************/
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	float fps;

/********************** text **********************/
	float FontData[256];
};

#endif