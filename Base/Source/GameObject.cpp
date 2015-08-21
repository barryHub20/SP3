#include "GameObject.h"
#include "Controller.h"
#include "MeshList.h"
#include "LoadTGA.h"
int GameObject::objCount = 0;

/*********************************** constructor/destructor ***********************************/
GameObject::GameObject(void)
{
	objCount++;
	SpriteName = "";
	SpriteCol = NULL;
	SpriteRow = NULL;
	Sprite_texture_file_path = "";
}

GameObject::GameObject(Vector3 Pos, Vector3 scale, Vector3 Dir, float Speed, bool active)
{
}

/*********************************** core ***********************************/
void GameObject::Update()
{
}

/*********************************** getter/setter ***********************************/
//active
void GameObject::setIsActive(bool isActive)
{
	Object::setActive(isActive);
}

bool GameObject::getisActive()
{
	return Object::getActive();
}

//type
void GameObject::setType(GO_TYPE Type)
{
	this->Type = Type;
}

int GameObject::getType()
{
	return Type;
}

//object count (static)
int GameObject::getObjCount()
{
	return objCount;
}

//return physics variables

Physics GameObject::getInfo()
{
	return info;
}

//dir
Vector3 GameObject::getDir()
{
	return Vector3(1, 0, 0);
}

//angle
void GameObject::setAngle(float angle)
{
	angleZ = angle;
	Object::rotateObject(angle, 0, 0, 1);
}

float GameObject::getAngle()
{
	return angleZ;
}

//mesh
void GameObject::setMesh(Mesh* mesh)
{
	Object::setMesh(mesh);
}

Mesh* GameObject::getMesh()
{
	return Object::getMesh();
}

//scale
Vector3 GameObject::getScale()
{
	return Object::getScale();
}

//pos
Vector3 GameObject::getPosition()
{
	return Object::getPosition();
}

float GameObject::getSpeed()
{
	return info.getSpeed();
}

void GameObject::setSpeed(float speed)
{
	info.setSpeed(speed);
}

Collision* GameObject::getCollideBound()
{
	return &collideBound;
}

/*********************************** utilities ***********************************/
void GameObject::StartCollisionCheck()
{
	collided = false;
	collideBound.Start(position);
}

bool GameObject::CollisionCheck(GameObject* checkWithMe)
{
	bool b = false;
	b = Collision::CheckCollision(this->collideBound, checkWithMe->collideBound);

	if(!collided)
		collided = b;

	return b;
}

void GameObject::CollisionResponse()
{
	if(collided)
	{
		translate(collideBound.position);
	}
}
void GameObject::storeSpriteAnimation(std::string name, unsigned int numRow, unsigned int numCol, const char * texture_file_path)
{
	this->SpriteName = name;
	this->SpriteRow = numRow;
	this->SpriteCol = numCol;
	this->Sprite_texture_file_path = texture_file_path;
}

SpriteAnimation* GameObject::generateSpriteMesh()
{
	SpriteAnimation *sa;
	sa = dynamic_cast<SpriteAnimation*>(MeshBuilder::GenerateSpriteAnimation(SpriteName, SpriteRow, SpriteCol, 1.f));
	sa->textureID[0] = LoadTGA(Sprite_texture_file_path);
	return sa;
}

void GameObject::processSpriteAnimation(int state, float time, int startCol, int startRow, int endCol, int endRow, int repeatCount, bool oppDir)
{
	//SpriteAnimation *sa;
	//sa = dynamic_cast<SpriteAnimation*>(MeshBuilder::GenerateSpriteAnimation(name, numRow, numCol, 1.f));
	//sa->textureID[0] = LoadTGA(texture_path);
	SpriteAnimation *temp;
	temp = generateSpriteMesh();
	if(temp->name == "" || temp->textureID[0] == 0)
	{
		cout << "Sprite empty" << endl;
		delete temp;
		return;
	}
	temp->init(time, startCol, startRow, endCol, endRow, repeatCount, oppDir);
	animationList[state] = temp;
}
void GameObject::Translate(Vector3 pos)
{
	translate(pos);
}

void GameObject::TranslateOffset(Vector3 offset)
{
	translateObject(offset);
}

GameObject::~GameObject(void)
{
	objCount--;
}
