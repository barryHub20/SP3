#include "Character.h"

const float floating_offset = 0.1f;

/* constructor / destructor */
Character::Character(){}
Character::~Character(){}

/* Core */
void Character::Init(SpriteAnimation* anim_mesh, float xPos, float yPos, float xScale, float yScale, Map* map)
{
	/* main stuff */
	pos.Set(xPos, yPos);
	velocity.Set(0, 0);
	scale.Set(xScale, yScale);
	
	/* hero mesh */
	this->anim_mesh = anim_mesh;
	invertAnim = false;

	/* map stuff */
	posOffset.Set(0, 0);

	/* flags */
	c_jumpingUp = false;
	c_jumpingDown = false;
	c_movingLeft = false;
	c_movingRight = false;
	active = false;

	/* const stuff */
	jumpSpeed = 0.f;
	moveSpeed = 0.f;

	/** Collide flags (FOr 4 sides) **/
	yCollide_origin = -1;
	yCollide_end = -1;
	xCollide_origin = -1;
	xCollide_end = -1;

	/* current values */
	checkPosition_X_Origin = (int)(pos.x / map->GetTileSize());
	checkPosition_Y_Origin = (int)(pos.y) / map->GetTileSize();
	checkPosition_X_End = (int)(pos.x + scale.x) / map->GetTileSize();
	checkPosition_Y_End = (int)(pos.y + scale.y) / map->GetTileSize();

	/* prev frame values (at start, 1st frame has no prev frame) */
	prevPosition_Y_Origin = checkPosition_Y_Origin;
	prevPosition_Y_End = checkPosition_Y_End;
}

/* collision */
void Character::Collide(const Vector2 offset, Map* map)
{
	/* collide flags */
	checkPosition_X_Origin = (int)((pos.x + offset.x) / map->GetTileSize());
	checkPosition_Y_Origin = (int)(pos.y) / map->GetTileSize();
	checkPosition_X_End = (int)((pos.x + offset.x) + scale.x) / map->GetTileSize();
	checkPosition_Y_End = (int)(pos.y + scale.y) / map->GetTileSize();

	/* X wall collide flags */
	left_Collide_Wall = false;
	right_Collide_Wall = false;

	/** Collide flags (FOr 4 sides) **/
	yCollide_origin = -1;
	yCollide_end = -1;
	xCollide_origin = -1;
	xCollide_end = -1;
	
	bool xPos_Checked = true;	//is x pos checked?
	bool yPos_Checked = true;	//is y pos checked?

	/********************************************** check Y collide **********************************************/
	float floating_offset = 0.1f;	//for floating point cal
	int _checkPosition_Y_Origin = checkPosition_Y_Origin;	//use this as updated checkPosition_Y since original to be used for X checking
	int _checkPosition_Y_End = checkPosition_Y_End;

	/* normal Y collide */
	for(int i = checkPosition_X_Origin; i <= checkPosition_X_End; ++i)
	{
		if(c_jumpingUp)
		{
			for(int j = prevPosition_Y_End; j <= _checkPosition_Y_End; ++j)
			{
				if(map->theScreenMap[j][i] > 0)
				{
					yCollide_end = j - 1;
					_checkPosition_Y_End = yCollide_end;
					break;
				}
			}
		}
		else if(c_jumpingDown)
		{
			for(int j = prevPosition_Y_Origin; j >= _checkPosition_Y_Origin; --j)
			{
				if(map->theScreenMap[j][i] > 0)
				{
					yCollide_origin = j + 1;
					_checkPosition_Y_Origin = yCollide_origin;
					break;
				}
			}
		}
	}
	/********************************************** check Y collide **********************************************/

	/********************************************** check X collide **********************************************/
	for(int i = checkPosition_Y_Origin; i <= checkPosition_Y_End; ++i)
	{
		if(c_movingLeft)
		{
			if(map->theScreenMap[i][checkPosition_X_Origin] > 0)
			{
				/* set collide tile value */
				xCollide_origin = checkPosition_X_Origin + 1;
				checkPosition_X_Origin++;
				break;
			}
		}
		else if(c_movingRight)
		{
			if(map->theScreenMap[i][checkPosition_X_End] > 0)
			{
				/* set collide tile value */
				xCollide_end = checkPosition_X_End - 1;
				checkPosition_X_End--;
				break;
			}
		}
	}
	/********************************************** check X collide **********************************************/
	//cout << xCollide_origin << ' ' << xCollide_end << ' ' << pos << ' ' << prevPos << endl;
	/********************************************** Freefall **********************************************/
	/** free fall: if  Y below it is not 0 and hero is not currently jumping up or down, jumping down == true **/
	bool freeFall = true;
	if(_checkPosition_Y_Origin - 1 >= 0 && (!c_jumpingUp && !c_jumpingDown) )	//only check if it is not on very first row
	{
		for(int i = checkPosition_X_Origin; i <= checkPosition_X_End; ++i)
		{
			/* if theres a block underneath it */
			if(map->theScreenMap[_checkPosition_Y_Origin - 1][i] > 0)
			{
				freeFall = false;
				break;
			}
		}

		/* if free fall, start jumping down */
		if(freeFall)
		{
			jumpSpeed = 0.f;
			c_jumpingDown = true;
		}
	}
	/********************************************** Freefall **********************************************/
	if(xCollide_origin > -1 || xCollide_end > -1)
		xPos_Checked = false;
	if(yCollide_origin > -1 || yCollide_end > -1)
		yPos_Checked = false;

	/* if both X and Y collides */
	if(!yPos_Checked && !xPos_Checked)
	{
		xPos_Checked = true;
	}

	if(yCollide_origin > -1 && yCollide_end > -1)
	{
		yPos_Checked = true;
	}

	/* if y collide, set jump flag */
	if(!yPos_Checked)
	{
		velocity.y = 0.f;
		jumpSpeed = 0.f;
	}

	if(yCollide_origin > -1)
	{
		c_jumpingDown = false;
	}
	else if(yCollide_end > -1)
	{
		c_jumpingUp = false;
		c_jumpingDown = true;
	}

	/*********************** Collision response ***********************/
	if(!xPos_Checked)
		checkXPos(xCollide_origin, xCollide_end, map, offset);

	if(!yPos_Checked)
		checkYPos(yCollide_origin, yCollide_end, map, offset);

	/* set the prev so that next frame can use it */
	prevPosition_Y_Origin = _checkPosition_Y_Origin;
	prevPosition_Y_End = _checkPosition_Y_End;
}

void Character::checkXPos(int _xCollide_origin, int _xCollide_end, Map* map, const Vector2 offset)
{
	velocity.x = 0.f;

	//X
	if(_xCollide_origin >= 0)	//moving left
	{
		pos.x = _xCollide_origin * map->GetTileSize() + floating_offset;
		pos.x -= offset.x;	//rmb to translate with offset
	}
	else if(_xCollide_end >= 0)	//moving right
	{
		pos.x = ( (_xCollide_end + 1) * map->GetTileSize()) - scale.x - floating_offset;
		pos.x -= offset.x;	//rmb to translate with offset
	}
}

void Character::checkYPos(int _yCollide_origin, int _yCollide_end, Map* map, const Vector2 offset)
{
	if(_yCollide_origin >= 0)	//jumping down
	{
		pos.y = map->GetTileSize() * _yCollide_origin + floating_offset;

	}
	else if(_yCollide_end >= 0)	//jumping up
	{
		pos.y = (map->GetTileSize() * (_yCollide_end + 1)) - scale.y - floating_offset;
	}
}
