#ifndef GOL_H
#define GOL_H
#include "Object.h"

//False: Dead cell
//True: Alive Cell

class GameOfLife
{
public:
	enum STATE
	{
		SETUP,	//set up the grids
		START_PLAYING,	//start playing itself
	};

	/* constructor/destructor */
	GameOfLife();
	~GameOfLife();

	/* core */
	void Set(short int row, short int column);
	void Update_SetUp(const double dt, Object* cursor);
	void Update(const double dt, Object* cursor);

private:
	vector<bool> grid;
	STATE state;
};


#endif