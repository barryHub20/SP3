#include "Model.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

unsigned short Model::m_view_width;	//camera view size X
unsigned short Model::m_view_height;	//camera view size Y
unsigned short Model::m_2D_view_width;
unsigned short Model::m_2D_view_height;
bool Model::bLightEnabled;
float Model::fps;
Position Model::lightPos[TOTAL_LIGHTS];
int Model::model_count = 0;
 int Model::current_model = 0;
 SoundManager* Model::sfx_man;

/*********** constructor/destructor ***************/
Model::Model()
{
	model_count++;
}

Model::~Model()
{
	delete sfx_man;
}

/*********** core functions ***************/
void Model::Init()
{
	sfx_man = new SoundManager();

	/* Must Init */
	InitMesh();

	bLightEnabled = true;

	/* Coord of screen */
	m_view_width = 1024.f;
	m_view_height = 800.f;

	/* Coord for UI */
	m_2D_view_width = 160.f;
	m_2D_view_height = 120.f;

	//Init sound
	sfx_man->sfx_init();
}

void Model::InitMesh()
{
	Geometry::Init();
}

void Model::Update(double dt, bool* myKeys, Vector3 mousePos)
{
	/* openGL stuff */
	UpdateOpenGL(dt, myKeys);

	/* Sprite animation */
	for(std::vector<SpriteAnimation*>::iterator it = Geometry::animation.begin(); it != Geometry::animation.end(); ++it)
	{
		SpriteAnimation *go = (SpriteAnimation *)*it;
		go->Update(dt);
	}

	fps = (float)(1.f / dt);

	if(fps < 58)
		cout << "FPS dropped to: " << fps << endl;
}

void Model::UpdateOpenGL(double dt, bool* myKeys)
{
	if(myKeys[KEY_1])
		glEnable(GL_CULL_FACE);
	if(myKeys[KEY_2])
		glDisable(GL_CULL_FACE);
	if(myKeys[KEY_3])
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(myKeys[KEY_4])
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Model::Exit()
{
	Geometry::Exit();
}

/*********** getter / setters ***************/
bool Model::getbLightEnabled(){return bLightEnabled;}
float Model::getFPS(){return fps;}
Camera2D* Model::getCamera(){return &camera;}
unsigned short Model::getViewWidth(){return m_view_width;}
unsigned short Model::getViewHeight(){return m_view_height;}
unsigned short Model::get2DViewWidth(){return m_2D_view_width;}
unsigned short Model::get2DViewHeight(){return m_2D_view_height;}
int Model::getModelCount(){return model_count;}
int Model::getCurrentModel(){return current_model;}
void Model::setCurrentModel(int i)
{
	if(i < model_count && i >= 0)
		current_model = i;
}

Position Model::getLightPos(int index)
{
	//if index exceeds
	if(index < 0 || index >= TOTAL_LIGHTS)
		return Position(0, 0, 0);
	else
		return lightPos[index];
}

bool Model::getWordFromString(string& sentence, string& word, char min, char max, int& index)
{
	bool returnMe = false;
	word = "";
	for(; index < sentence.length(); ++index)	//loop through
	{
		/* get word based on min and max har*/
		
		if(sentence[index] >= min && sentence[index] <= max)	
		{
			word += sentence[index];	//get the whole word
			returnMe = true;
		}
		else if(returnMe)	//if reach end of min max char
			return true;
		
		if(index == sentence.length() - 1 && returnMe)	//last char and is still min max range
		{
			++index;
		
			return true;
		}
	}
	return false;
}

float Model::stringTofloat(string& number)
{
	/* interpret any ascii at front not 48-57 as minus sign */
	float returnVal = 0;
	int stopIndex = 0;
	float multiplier = 1;

	if(number[0] == '-')
		stopIndex = 1;

	/* check if got floating point */
	int floatPoint_total = -1;	//get index
	for(int i = number.length() - 1; i >= stopIndex; --i)
	{
		if(number[i] == '.')	//floating point
		{
			floatPoint_total = (number.length() - 1) - i;
			break;
		}
	}

	/* set the multiplier to starting value */
	if(floatPoint_total != -1)
	{
		for(int i = 0; i < floatPoint_total; ++i)
			multiplier /= 10.f;
	}


	/* calculate num */
	for(int i = number.length() - 1; i >= stopIndex; --i)
	{
		if(number[i] >= 48 && number[i] <= 57)
		{
			returnVal += (number[i] - 48) * multiplier;
			multiplier *= 10;
		}
	}

	if(stopIndex > 0)
	{
		return returnVal * -1;
	}
	else
	{
		return returnVal;

	}
}

