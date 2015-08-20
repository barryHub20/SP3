#ifndef OBJECT_H
#define OBJECT_H

#include "Mesh.h"
#include "Mtx44.h"
#include <vector>
#include <string>
using namespace std;

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

/***********************************************************

Class Object: the physical part of a in-game object
eg. for a car type, Object is its physical form, including relevant data

param:
	Mesh* mesh: mesh to render
	Vector3 scale: scale factor
	Vector3 position: position
	BoundBox bbox: AABB box
	Object* parent: parent object, to transform along with it like herichical modeling, NULL for no parent
	Mtx44 TRS: the entire transformation matrix for this object, which is transformed along with parent if theres one

***********************************************************/
class Object
{
protected:
	/*** modifyable data ***/
	Mesh* mesh;	//mesh to render (need?)
	Vector3 scale;	//scale
	Vector3 position;	//pos
	Object* parent;	
	bool light;		//have light?
	bool shadow;	//have shadow?
	bool active;	//active?

	/*** NON-modifyable data ***/
	//BoundBox_3D bbox;	//Not supposed to be here
	Mtx44 TRS;
public:
	/*** constructor / destructor ***/
	Object();
	virtual ~Object();

	/*** core ***/
	void Set(string name, Mesh* mesh, Object* parent, bool light, bool shadow);
	void AddParent(Object* parent);
	void Init();

	/* particle */
	virtual void Update(const double& dt, Vector3& playerPos){};

	/*** utilities ***/
	void transformWithParent();	//position recalculated after transformation
	//void checkCollision(Object& checkObject);	//if collide, bounce back

	/*** transformation ***/
	void scaleObject(float x, float y, float z);
	void scaleObject(float all);
	void translateObject(float x, float y, float z);
	void translateObject(Vector3 pos);
	void rotateObject(float angle, float xAxis, float yAxis, float zAxis);
	void translate(const Vector3& pos);
	void translate(float x, float y, float z);

	/*** getters setter ***/
	Mesh* getMesh();
	void setMesh(Mesh* mesh);
	Vector3 getScale();
	Vector3 getPosition();
	Object* getParent();
	//BoundBox_3D* getBbox();
	Mtx44* getTRS();
	bool getLight();
	void setActive(bool b);
	bool getActive();
	void setShadow(bool s);
	bool getShadow();
};


#endif