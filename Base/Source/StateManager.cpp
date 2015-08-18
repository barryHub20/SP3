#include "StateManager.h"
#include <iostream>


void StateManager::UpdateTransitionTime(float dt)
{
	transitionTime += dt;
	if (transitionTime > 0.1f)
	{
		resetTransition();
		if (stateToChangeTo != NO_STATE)
		{
			setState(stateToChangeTo);
		}
		stateToChangeTo = NO_STATE;
	}	
}

bool StateManager::isTransition()
{
	return transition;
}

StateManager::StateManager()			// Constructor
{
	state = NO_STATE;
	stateToChangeTo = NO_STATE;
	transitionTime = 0.f;
	transition = false;
}


StateManager::~StateManager()
{
}

StateManager::STATES StateManager::GetState()
{
	return state;
}

void StateManager::ChangeState(STATES state)
{
	std::cout << "Switching state with transition" << std::endl;
	stateToChangeTo = state;
	setTransition();
	transition = true;
}

void StateManager::SetTransitionTime(float transitionTime)
{
	this->transitionTime = transitionTime;
}

void StateManager::setTransition()
{
	this->state = TRANSITION;
}

void StateManager::resetTransition()
{
	transition = false;
	transitionTime = 0.f;
}

void StateManager::setState(STATES state)
{
	this->state = state;
}
