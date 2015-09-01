#ifndef MODEL_LEVEL2_H
#define MODEL_LEVEL2_H
#include "Model_Level.h"

class Model_Level2 : public Model_Level
{
private:
	enum LEVEL_STATE
	{
		STAGE_1,
		/*  Tutorial: pick up the fking note on the floor
			Item 1 - > UI Object 1: picture of boxes in this */

		STAGE_2,
		/*  Tutorial: grab the key and the new clue
			Item 2 - > UI Object 2: pull lever
			Item Key 1: unlock door 1 */

		STAGE_3,
		/*  pull lever
		*/

		STAGE_4,
		/*  Item Key 2: unlock door 2 and to FREEDOM(lvl 3) */

		TOTAL_STAGE,
	};

/*********** Puzzle system ***************/
	LEVEL_STATE level_state;	//for scripting/puzzle

	//trigger for hidden puzzles and clues
	TriggerArea puzzleActivateArea;	//activate to get hidden key and clue in stage 2
	Collision stage2Area;	//stage 2 area
	Collision stage3Area;	//stage 3 area

	//keys
	//bool pickedUpKeys[2];
	//bool pickedUpNotes[2];
	Item* puzzleNotes[2];
	Item* puzzleKeys[2];

	//lever to remove traps
	GameObject* leverClose;
	GameObject* leverOpen;

	//spike trap
	GameObject* spikeTrap;

	//doors
	GameObject* doors[2];
public:
/*********** constructor/destructor ***************/
	Model_Level2();
	~Model_Level2();

/*********** core functions ***************/
	//Initialize
	virtual void Init();
	void InitTrigger();	//trigger for puzzle
	void InitObject();
	void InitPuzzles();
	void spawnItems();

	//Update
	virtual void Update(double dt, bool* myKeys, Vector3 mousePos);
		void UpdateGame(double dt, bool* myKeys);
		void UpdatePuzzle(double dt, bool* myKeys);
		void UpdateCollision();	//anything that needs to be collided, put here
		void UpdateEnemy(double dt);


	virtual void Exit();
};

#endif