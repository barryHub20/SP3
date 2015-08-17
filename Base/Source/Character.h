#ifndef CH_H
#define CH_H
#include"Vector2.h"
#include "Object.h"
#include "MeshList.h"
#include "Map.h"

/** Put here for controller and model and character to access, got better way? **/
enum KEYS
{
	/* keyboard */
	KEY_B,
	KEY_V,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_C,
	KEY_T,
	KEY_K,
	KEY_L,
	KEY_I,
	KEY_O,
	KEY_SPACE,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,

	/* non-keyboard */
	KEY_LMOUSE,
	KEY_RMOUSE,
	TOTAL_KEY,
};
/** Put here for controller and model to access, got better way? **/

class Character
{
public:
	/* constructor/destructor */
	Character();
	virtual ~Character();

	/* Core */
	virtual void Init(SpriteAnimation* anim_mesh, float xPos, float yPos, float xScale, float yScale, Map* map);
	//virtual void Update(double dt, bool* myKeys, float m_window_width, float m_window_height, Vector2& middlePos, const float constrain_DistX, Vector2& offset, Map* map) = 0;

	/* collision */
	void Collide(const Vector2 offset, Map* map);

	void checkXPos(int yCollide_origin, int yCollide_end, Map* map, const Vector2 offset);
	void checkYPos(int xCollide_origin, int xCollide_end, Map* map, const Vector2 offset);

	/* movement */
	virtual void Jump(double dt) = 0;
	virtual void MoveLeftRight(bool mode, double dt) = 0;

	/************ Player ************/
	Vector2 pos;
	Vector2 velocity;
	Vector2 prevPos;	//for collision
	Vector2 scale;
	Vector2 posOffset;	//for 2D tilemap offset

	float jumpSpeed;
	float moveSpeed;

	/* flags */
	bool c_jumpingUp;
	bool c_jumpingDown;
	bool c_movingLeft;
	bool c_movingRight;
	bool right_Collide_Wall;	//collides a wall right	
	bool left_Collide_Wall;		//collides a wall left
	bool active;

	/* invert anim */
	bool invertAnim;
	SpriteAnimation* anim_mesh;

	/** Collide flags (FOr 4 sides) **/
	int yCollide_origin;
	int yCollide_end;
	int xCollide_origin;
	int xCollide_end;

	/* check Pos */
	//get the current collided tiles for Start and End of character
	int checkPosition_X_Origin;
	int checkPosition_Y_Origin;
	int prevPosition_Y_Origin;
	int prevPosition_Y_End;

	//get the previous frame collided tiles for Start and End of character
	int checkPosition_X_End;
	int checkPosition_Y_End;
};


#endif