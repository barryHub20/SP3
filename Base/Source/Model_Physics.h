#ifndef MODELPHY_H
#define MODELPHY_H
#include "Model.h"

class Model_Physics : public Model
{
private:
/*************particles: Weather *****************/
	float lengthFromPlayer;	//4 sides: front back left right
	float howManyParticles;	//how many particles per length
	float initialY;	//fixed Y spawning pos
	Vector3 pos_offset;	//camera pos offset
	Vector3 initial_pos;	//initial camera position
	int start_Index, end_Index;	//for snow
public:
/*********** constructor/destructor ***************/
	Model_Physics();
	~Model_Physics();

/*********** core functions ***************/
	virtual void Init();
	void InitObject();
	void InitBillboard();
	void InitSnow();

	virtual void Update(double dt, bool* myKeys);
	void UpdateLight(double dt, bool* myKeys, Light* light);	//get light from view
	void UpdateBillboard();
	void UpdateSnow(double dt);

	virtual void Exit();
};

#endif