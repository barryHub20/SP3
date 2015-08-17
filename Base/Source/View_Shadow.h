#ifndef VIEW_SHADOW_H
#define VIEW_SHADOW_H
#include "Model.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

/**
 Everything in view class will be using Objects class for TRS and other info
 **/
class View_Shadow
{
private:
/********************** openGL *********************************/
	static GLFWwindow* m_window_view;

/********************** model and view ptr **********************/
	Model* model;

public:
/********************** Window screen size *****************************/
	//dimension on computer screen
	const static unsigned short m_window_width = 800;
	const static unsigned short m_window_height = 600;

	//unit
	const static unsigned short m_window_width_2D = 80;
	const static unsigned short m_window_height_2D = 60;

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

		/* Shadow */
		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,

		U_TOTAL,
	};

	//SHADOW: RENDER_PASS
	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};

	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	RENDER_PASS m_renderPass;

	enum MODE
	{
		TWO_D,
		THREE_D,
	};

/********************** constructor/destructor *****************************/
	View_Shadow();
	View_Shadow(Model* model);
	~View_Shadow();

/********************** Core functions *****************************/
	void Init(MODE mode);
	void InitLight();
	void InitFontData();
	void InitProjection();
	void Render(const float fps);
	void Exit();

/**************** render ****************/
	//main wrapper
	void Render3D();
	void RenderPassGPass();
	void RenderPassMain();

	void Render2D();

	void RenderLight();
	void RenderObject();
	void RenderCollideBox();
	void RenderHUD();

	/* If theres TileMap */
	void RenderTileMap();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size=1.0f, float x=0.0f, float y=0.0f, float z = 0.f, float angle = 0.f);
	void Render3DonScreen(Mesh* mesh, Camera3* cam, bool enableLight, Vector3 scale);
	void RenderMeshInLines(Mesh* mesh, const Vector3& position, const Vector3& scale);
	void Render2DMesh(Mesh *mesh, bool enableLight, float sizeX, float sizeY, float x, float y);
	void Render2DTile(Mesh *mesh, bool enableLight, float size, float x, float y, int tileType);

/********************** Window screen size *****************************/
	static int getWindowHeight();
	static int getWindowWidth();

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