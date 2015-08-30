#ifndef MESHLIST_H
#define MESHLIST_H
#include "Mesh.h"
#include "MeshBuilder.h"
#include "DepthFBO.h"
#include <vector>

using std::vector;

class Geometry
{
public:
	enum GEOMETRY_TYPE
	{
		/* basic ABC */
		GEO_AXES,
		GEO_DEBUG_CUBE,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_QUAD,
		GEO_CUBE,
		GEO_RING,
		GEO_CONE,

		/* sky boxxxxxxxxxxx */
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		/* UI */
		GEO_AR_CHRISTY,	//font
		GEO_CROSSHAIR,
		GEO_SLOT,
		GEO_SELECTOR,

		/* terrain */
		GEO_SKYPLANE,
		GEO_TERRAIN,

		/* animation */
		GEO_GUARD,

		/* world objects */
		GEO_BUILDING,
		GEO_OBJECT,
		GEO_PAVEMENT,
		GEO_SIGNBOARD,
		GEO_LAMPPOST,
		GEO_SNOWFLAKE,

		/* police car */
		GEO_CAR_MAIN_PART,
		GEO_CAR_GLASS,
		GEO_CAR_SIREN,

		/* building */
		GEO_DERELICT_BUILDING_01,

		/* 2D */
		GEO_TILEMAP,
		GEO_DUNGEONTILE,
		GEO_TILESET1,
		GEO_TEMPFLOOR,
		GEO_JINFLOOR,
		IMAGE_TITLE,
		GEO_LSTAIRCASEUP,
		GEO_RSTAIRCASEDOWN,
		GEO_RSTAIRCASEUP,
		GEO_LSTAIRCASEDOWN,

		GEO_KEYY,
		GEO_KEYR,
		GEO_KEYG,
		GEO_KEYP,
		GEO_HPOTION,
		GEO_SPOTION,
		GEO_ATKPOTION,
		GEO_DOORR,
		GEO_DOORP,
		GEO_DOORG,
		GEO_DOORY,
		GEO_ISTRIGGER,
		GEO_NOTTRIGGER,
		GEO_FIRE,
		GEO_ARROWUP,
		GEO_ARROWDOWN,
		GEO_ARROWLEFT,
		GEO_ARROWRIGHT,

		/* HUD */
		GEO_HEALTHBARBG,
		GEO_HEALTHBARCOLOR,
		GEO_HEALTHBARMARKER,
		GEO_STAMINABARCOLOR,
		GEO_STAMINABARMARKER,
		GEO_CURSOR,
		GEO_BACKGROUND,
		GEO_MAIN_BAR,	//main UI bar

		NUM_GEOMETRY,
	};

	/************ meshlsit ************/ 
	static Mesh* meshList[NUM_GEOMETRY];	//dynamic array for Mesh*

	/************ animation ************/ 
	static vector<SpriteAnimation*> animation;

	/************ terrain ************/
	static vector<unsigned char> m_heightMap;

	/* core */
	static void Init();	//MUST BE CALLED ONCE: ANYWHERE
	static void Exit();
};


#endif