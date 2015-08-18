#ifndef MODEL2D_H
#define MODEL2D_H
#include "Model.h"

class Model_2D : public Model
{
	GameObject* test_obj;
public:
/*********** constructor/destructor ***************/
	Model_2D();
	~Model_2D();

/*********** core functions ***************/
	virtual void Init();
	void InitObject();
	void InitMaps(); //Initialize maps (RP)

	virtual void Update(double dt, bool* myKeys);
	void UpdateLight(double dt, bool* myKeys, Light* light);	//get light from view

	virtual void Exit();
};

#endif