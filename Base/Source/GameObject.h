#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Object.h"
#include "Physics.h"
#include "Collision.h"
#include "MeshList.h"
#include "SpriteAnimation.h"
#include <map>
#include "MyMath.h"

class GameObject : public Object
{
private:
	std::string SpriteName;
	unsigned int SpriteCol;
	unsigned int SpriteRow;
	const char * Sprite_texture_file_path;
public:

	/* object type */
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
	/* constructor/destructor */
	GameObject(void);
	GameObject(Vector3 Pos, Vector3 scale, Vector3 Dir, float Speed, bool active);	//set everything in general
	virtual ~GameObject(void);

	/* Core */
	virtual void Update();
	
	void setInfo(float Speed);	//Set physics info only
	Physics getInfo();

	/* getter/setter */
	void setIsActive(bool isActive);
	bool getisActive();

	Vector3 getPosition();
	Vector3 getScale();

	void setType(GO_TYPE Type);
	int getType();

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

	/* !! angle is only rotating along z axis */
	void setAngle(float angle);
	float getAngle();

	float getSpeed();
	void setSpeed(float speed);

	Vector3 getDir();

	Collision* getCollideBound();

	/* utilities */
	void Translate(Vector3 pos);
	void TranslateOffset(Vector3 offset);

	virtual void StartCollisionCheck();	//set-up collision bound for checking
	bool CollisionCheck(GameObject* checkWithMe);	//collision check
	virtual void CollisionResponse();

	//Stores the values for the sprite sheet so the class can use it later on
	virtual void storeSpriteAnimation(std::string name, unsigned int numRow, unsigned int numCol, const char * texture_file_path);

	//Creates a line of sprite and adds it to the state
	virtual void processSpriteAnimation(int state, float time, int startCol, int startRow, int endCol, int endRow, int repeatCount, bool oppDir = false);

	static int getObjCount();

protected:

	GO_TYPE Type;	//type
	Physics info;	// Physics related info
	Collision collideBound;	//collision box
	float angleZ;	//angle rotate on z axis (z faces outwards from screen)
	bool collided;

	static int objCount;
	
	std::map<int, SpriteAnimation*> animationList;

	virtual SpriteAnimation* generateSpriteMesh(); //Creates a sprite animation from the stored values
};

#endif 

