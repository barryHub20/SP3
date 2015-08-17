#include "PoliceCruiser.h"

/* constructor / destructor */
PoliceCruiser::PoliceCruiser()
{
}

PoliceCruiser::PoliceCruiser(const PoliceCruiser& copy)
{
}

PoliceCruiser::~PoliceCruiser()
{
}

/* core */
void PoliceCruiser::Init(Vector3& pos, vector<Object*>& objectList)
{
	type = POLICE_CRUISER;
	object_array = new Object*[TOTAL_PART];

	//Vehicle::Init(pos);
	//objectList is the model object list that is passed on to render
	/* main body */
	object_array[MAIN_BODY] = new Object;
	object_array[MAIN_BODY]->Set("Main body", Geometry::meshList[Geometry::GEO_SPHERE], Vector3(1, 1, 1), pos, NULL, false, false);
	object_array[MAIN_BODY]->scaleObject(20, 20, 20);
	objectList.push_back(object_array[MAIN_BODY]);
	
	/* siren */
	object_array[SIREN] = new Object;
	object_array[SIREN]->Set("Siren", Geometry::meshList[Geometry::GEO_SPHERE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);
	//object_array[SIREN]->scaleObject(20, 120, 20);
	object_array[SIREN]->translateObject(0, 2, 0);
	object_array[SIREN]->AddParent(object_array[MAIN_BODY]);
	objectList.push_back(object_array[SIREN]);

	///* headlight */
	object_array[HEADLIGHT] = new Object;
	object_array[HEADLIGHT]->Set("Headlight", Geometry::meshList[Geometry::GEO_SPHERE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);
	object_array[HEADLIGHT]->translateObject(2, 0, 0);
	object_array[HEADLIGHT]->AddParent(object_array[MAIN_BODY]);
	objectList.push_back(object_array[HEADLIGHT]);

	/* taillight */
	object_array[TAILLIGHT] = new Object;
	object_array[TAILLIGHT]->Set("Headlight", Geometry::meshList[Geometry::GEO_SPHERE], Vector3(1, 1, 1), Vector3(0, 0, 0), NULL, false, false);
	object_array[TAILLIGHT]->translateObject(-2, 0, 0);
	object_array[TAILLIGHT]->AddParent(object_array[MAIN_BODY]);
	objectList.push_back(object_array[TAILLIGHT]);
}

void PoliceCruiser::Update()
{
}

void PoliceCruiser::translate(float x, float y, float z)
{
	object_array[MAIN_BODY]->translate(x, y, z);
}