#include "GameOfLife.h"

/* constructor/destructor */
GameOfLife::GameOfLife(){}
GameOfLife::~GameOfLife(){}

/* core */
void GameOfLife::Set(short int row, short int column)
{
	int total = row * column;

	for(int i = 0; i < total; ++i)
	{
		grid.push_back(false);
	}

	state = SETUP;	//setup state
}

vector<bool>::iterator gol_it;
void GameOfLife::Update_SetUp(const double dt, Object* cursor)
{
	/* if collide and click: change state */
	for(gol_it = grid.begin(); gol_it != grid.end(); ++gol_it)
	{

	}
}

void GameOfLife::Update(const double dt, Object* cursor)
{
	
}