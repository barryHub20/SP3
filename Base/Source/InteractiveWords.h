#ifndef IWORD_H
#define IWORD_H
#include "Object.h"
#include <vector>
#include <string>
using namespace std;

/** 
	For changing text/images on digital screens
	In future: change this class to inherit from object class and object class has a render function
**/
class IWords
{
public:
	enum TYPE
	{
		SMOKING,
		ZOO,
	};

	/* constructor / destructor */
	//constructor for base class Object
	//!! Mesh: Must be GEO_TEXT
	IWords();
	virtual ~IWords();

	/** core **/
	void Set(Mesh* mesh, Vector3 scale, Vector3 position, float angle, Vector3 axis, float mass, Object* parent, bool light, TYPE type);
	void Init(double changeRate);

	/** getter setter **/
	void Update(const double dt);
	Object* getObject();
	string getDisplayString();
	bool getRenderMesh();

private:
	/* variables */
	double changeRate;
	double changeTimer;

	//phyiscal objects
	Object object;	//physical object
	string displayWord;	//word to display
	
	// bools and trackers
	TYPE type;
	int index;	//keeps track of words: eg. show mesh passed in (like at 2nd word show 'no cig')
	bool renderMesh;	//true to render mesh

	/* internal variables */
	//smoking ad
	const static string noToCigs[2];
	const static unsigned noToCigsSize = 2;

	//zoo
	const static string zoo[3];
	const static unsigned zooSize = 3;
};


#endif