#include "pch.h"
#include "DebugStateMachine.h"
#include "State.h"
#include "StateTransition.h"
#include "StateMachine.h"
#include <iostream>
NCL::CSC8599::DebugStateMachine::DebugStateMachine()
{
	auto A = new StateMachine("0",
		new State([this](float dt)->void
		{
			time_stack = 0.0f;
		}));
	A->AddComponent("1", new State([this](float dt)->void
		{
			time_stack += dt;
			if (time_stack > 1.5f)
				EventSystem::Get()->PushEvent("Debug_PetTaunt", 0);
		}));
	A->AddTransition(new StateTransition(A->GetComponent("0"), A->GetComponent("1"), [this](EVENT* event)->bool
	{
			return EventSystem::Get()->HasHappened("PlayerOverThreat");
		/*&&
				(EventSystem::Get()->HasHappened("PlayerDie")||
					!EventSystem::Get()->HasHappened("Debug_PetTaunt"))
				use LTL_TO_EDGES 
		*/
	},""));
	A->AddTransition(new StateTransition(A->GetComponent("1"), A->GetComponent("0"), [this](EVENT* event)->bool
	{
			return EventSystem::Get()->HasHappened("Debug_PetTaunt");
		/*&&
				!EventSystem::Get()->HasHappened("PlayerDie")

		*/
	}, ""));


	auto B = new StateMachine("2",
		new State([this](float dt)->void
		{}));
	B->AddComponent("1", new State([this](float dt)->void
		{}));
	B->AddComponent("0", new State([this](float dt)->void
		{
			EventSystem::Get()->PushEvent("Debug_PetDie", 0);
		}));

	B->AddTransition(new StateTransition(B->GetComponent("2"), B->GetComponent("1"), [this](EVENT* event)->bool
		{
		return EventSystem::Get()->HasHappened("PlayerDie");
		}, ""));
	B->AddTransition(new StateTransition(B->GetComponent("1"), B->GetComponent("2"), [this](EVENT* event)->bool
		{
			return true;
		}, "PetDie"));

	B->AddTransition(new StateTransition(B->GetComponent("1"), B->GetComponent("0"), [this](EVENT* event)->bool
		{
		return !EventSystem::Get()->HasHappened("PetDie");
		}, ""));
	B->AddTransition(new StateTransition(B->GetComponent("0"), B->GetComponent("2"), [this](EVENT* event)->bool
		{
			return true;
		}, "PetDie"));
	//AddComponent("DebugA", A);
	AddComponent("DebugB", B);
}

NCL::CSC8599::DebugStateMachine::~DebugStateMachine()
= default;