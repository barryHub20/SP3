#ifndef POLICECRUISER_H
#define POLICECRUISER_H
#include "Vehicle.h"

class PoliceCruiser : public Vehicle
{
public:
	enum PARTS
	{
		MAIN_BODY,
		SIREN,
		HEADLIGHT,
		TAILLIGHT,
		TOTAL_PART,
	};

	/* constructor / destructor */
	PoliceCruiser();
	PoliceCruiser(const PoliceCruiser& copy);
	virtual ~PoliceCruiser();

	/* core */
	virtual void Init(Vector3& pos, vector<Object*>& objectList);
	virtual void Update();

	virtual void translate(float x, float y, float z);
};

#endif

