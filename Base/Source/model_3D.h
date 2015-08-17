#ifndef MODEL3D_H
#define MODEL3D_H
#include "Model.h"

class Model_3D : public Model
{
public:
/*********** constructor/destructor ***************/
	Model_3D();
	~Model_3D();

/*********** core functions ***************/
	virtual void Init();
	void InitObject();

	virtual void Update(double dt, bool* myKeys);
	void UpdateLight(double dt, bool* myKeys, Light* light);	//get light from view

	virtual void Exit();
};

#endif