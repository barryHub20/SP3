#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Physics.h"
#include "Object.h"
#include "MeshList.h"

/** Put here for controller and model and character to access, got better way? **/
enum KEYS
{
	/* keyboard */
	KEY_B,
	KEY_V,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_C,
	KEY_T,
	KEY_K,
	KEY_L,
	KEY_I,
	KEY_O,
	KEY_SPACE,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,

	/* non-keyboard */
	KEY_LMOUSE,
	KEY_RMOUSE,
	TOTAL_KEY,
};

class GameObject
{
	public:

	enum GO_TYPE
	{
		GO_START,
		GO_FURNITURE,
		GO_ITEM,
		GO_TILE,
		GO_PLAYER,
		GO_ENEMY,
		GO_CAMERA,
		GO_LASER,
		GO_TOTAL,
	};


	/* Set physics info only */
	void setInfo(float Speed);

	virtual void Update();
	void setIsActive(bool isActive);
	void setType(GO_TYPE Type);

	/* getter/setter */
	Object* getObject();
	Physics getInfo();
	int getObjCount();
	bool getisActive();
	int getType();
	
	GameObject(void);

	/* set everything in general */
	GameObject(Vector3 Pos, Vector3 scale, Vector3 Dir, float Speed, bool active);
	virtual ~GameObject(void);

private:
	Object object;
	Physics info;
	GO_TYPE Type;
	//Collision collide;

	static int objCount;
};

#endif 

