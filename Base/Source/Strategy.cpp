#include "Strategy.h"

/* constructor/destructor */
Strategy::Strategy()
{
	destination.Set(0, 0);
	going_Left = false;
	going_Right = false;
	jumping_Up = false;
	jumping_Down = false;
	fireLeft = fireRight = false;
}

Strategy::~Strategy()
{
}

/* Getter/setter */
Vector2 Strategy::GetDestination(){return destination;}
