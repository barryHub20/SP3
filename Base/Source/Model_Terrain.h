#ifndef MODELTERRAIN_H
#define MODELTERRAIN_H
#include "Model.h"
class ModelTerrain : public Model
{
private:
	//Lab prac 2: Game logic
	Vector3 playerPos;
public:
/*********** constructor/destructor ***************/
	ModelTerrain();
	~ModelTerrain();

/*********** core functions ***************/
	virtual void Init();
	void InitObject();
	void InitTileMap();
	void InitHero();

	virtual void Update(double dt, bool* myKeys);
	void UpdateHero(double dt, bool* myKeys, float m_window_width, float m_window_height);
	void UpdateTileMap(double dt, float m_window_width, float m_window_height);
	void UpdateLight(double dt, bool* myKeys, Light* light);	//get light from view

	virtual void Exit();
};

#endif