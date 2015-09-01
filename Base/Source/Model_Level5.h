#ifndef MODEL_LEVEL5_H
#define MODEL_LEVEL5_H
#include "Model_Level.h"

class Model_Level5 : public Model_Level
{
private:
	enum INSTRUCTION_MANUAL
	{
		IM_1,
		/*  Tutorial: pick up the fking note on the floor
			Item 1 - > UI Object 1: picture of boxes in this */

		IM_2,
		/*  Tutorial: grab the key and the new clue
			Item 2 - > UI Object 2: pull lever
			Item Key 1: unlock door 1 */

		IM_3,
		/*  pull lever
		*/

		IM_4,
		/*  Item Key 2: unlock door 2 and to FREEDOM(lvl 3) */

		TOTAL_STAGE,
	};

/*********** Puzzle system ***************/
	INSTRUCTION_MANUAL State;	//for scripting/puzzle

	//trigger for hidden puzzles and clues
	vector<TriggerArea*> TriggerAreaList;	//activate to get hidden key and clue in stage 2

	//Arrow
	GameObject* Arrow;
	//lever to remove traps
	GameObject* leverClose;
	GameObject* leverOpen;

	//spike trap
	GameObject* spikeTrap;

	//doors
	GameObject* doors[2];

	//keys
	Item* keyList[3];
public:
/*********** constructor/destructor ***************/
	Model_Level5();
	~Model_Level5();

/*********** core functions ***************/
	//Initialize
	virtual void Init();
	void InitTrigger();	//trigger for puzzle
	void InitObject();
	void InitPuzzles();
	void spawnItems();

	//Update
	virtual void Update(double dt, bool* myKeys, Vector3 mousePos, StateManager::STATES currentState);
		void UpdateGame(double dt, bool* myKeys);
		void UpdatePuzzle(double dt, bool* myKeys);
		void UpdateEnemy(double dt);
		void UpdateTraps(double dt, bool* myKeys);

	virtual void Exit();
};

#endif