#include "pch.h"
#include "DebugStateMachine.h"
#include "State.h"
#include "StateTransition.h"
NCL::CSC8599::DebugStateMachine::DebugStateMachine()
	:StateMachine("0", new State([this](float dt)->void
	{
		time_stack = 0.0f;
	}))
{
	AddComponent("1", new State([this](float dt)->void
	{
		time_stack += dt;
		if (time_stack > 0.5f)
			EventSystem::Get()->PushEvent("PetTaunt", 0);
	}));
	AddTransition(new StateTransition(GetComponent("0"), GetComponent("1"), [this](EVENT* event)->bool
	{
			return EventSystem::Get()->HasHappened("PlayerOverThreat")&&
				(EventSystem::Get()->HasHappened("PlayerDie")||
					!EventSystem::Get()->HasHappened("PetTaunt"));
	},""));
	AddTransition(new StateTransition(GetComponent("1"), GetComponent("0"), [this](EVENT* event)->bool
	{
			return EventSystem::Get()->HasHappened("PetTaunt")&&
				!EventSystem::Get()->HasHappened("PlayerDie");
	}, ""));
}

NCL::CSC8599::DebugStateMachine::~DebugStateMachine()
= default;