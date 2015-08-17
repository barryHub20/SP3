#include "InteractiveWords.h"
const string IWords::noToCigs[2] = {"TO", "SMOKE"};
const string IWords::zoo[3] = {"HI", "IM", "BOB"};

/* constructor / destructor */
IWords::IWords()
{
}

IWords::~IWords()
{
}

/** core **/
void IWords::Set(Mesh* mesh, Vector3 scale, Vector3 position, float angle, Vector3 axis, float mass, Object* parent, bool light, TYPE type)
{
	/* set the object */
	object.Set("fighter plane", mesh, scale, position, parent, light);
	this->type = type;
	changeRate = 1.0;
	changeTimer = 0;
	index = 0;
	displayWord = "";
	
	renderMesh = false;
}

void IWords::Init(double changeRate)
{
	//Init object
	object.Init();

	switch(type)
	{
	case SMOKING:
		 {
			  displayWord = noToCigs[0];
			  break;
		  }
	case ZOO:
		{
			displayWord = zoo[0];
			break;
		}
	}
}

void IWords::Update(const double dt)
{
	//reset object
	object.Reset();

	/** timer **/
	if(changeTimer < changeRate)
		changeTimer += dt;

	/** change words **/
	else
	{
		changeTimer = 0;
		++index;

		switch(type)
		{
		case SMOKING:
			 {
				 //display image
				 if(index >= noToCigsSize)
				 {
					 displayWord = "";
					 renderMesh = true;
					if(index >= noToCigsSize + 1)
					{
						index = 0;
						 renderMesh = false;
					}
				 }
				 

				 if(index < noToCigsSize)
					displayWord = noToCigs[index];

				break;
			 }
		case ZOO:
			{
				//zoozooSize
				if(index >= zooSize)
					index = 0;

				displayWord = zoo[index];

				break;
			}
		}
	}

	//update object
	object.Update();
}

Object* IWords::getObject(){return &object;}
string IWords::getDisplayString(){return displayWord;}
bool IWords::getRenderMesh(){return renderMesh;}