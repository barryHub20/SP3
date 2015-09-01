#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H



class StateManager
{
public:
	enum STATES
	{
		NO_STATE,
		MAIN_MENU,
		TRANSITION,
		GAME, //maps
		HIGHSCORE,
		INSTRUCTION,
		MAX_STATES,
	};
private:
	STATES state;
	STATES stateToChangeTo;
	float transitionTime;
	bool transition;
protected:
	void setTransition();
	void resetTransition();
public:

	StateManager();
	~StateManager();

	void setState(STATES state);
	StateManager::STATES GetState();
	void ChangeState(STATES state);
	void SetTransitionTime(float transitionTime);
	void UpdateTransitionTime(float dt);
	bool isTransition();
};

#endif