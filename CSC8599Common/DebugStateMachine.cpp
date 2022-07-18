#include "pch.h"
#include "DebugStateMachine.h"

#include "State.h"
#include "StateTransition.h"
#include "StateMachine.h"


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
	A->AddTransition(new CSC8599::StateTransition(A->GetComponent("0"), A->GetComponent("1"), [this](EVENT* event)->bool
	{
			return EventSystem::Get()->HasHappened("PlayerOverThreat");
		/*&&
				(EventSystem::Get()->HasHappened("PlayerDie")||
					!EventSystem::Get()->HasHappened("Debug_PetTaunt"))
				use LTL_TO_EDGES 
		*/
	},""));
	A->AddTransition(new CSC8599::StateTransition(A->GetComponent("1"), A->GetComponent("0"), [this](EVENT* event)->bool
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
	{
	}));
	B->AddComponent("0", new State([this](float dt)->void
		{
			EventSystem::Get()->PushEvent("Debug_PetDie", 0);
		}));

	B->AddTransition(new CSC8599::StateTransition(B->GetComponent("2"), B->GetComponent("1"), [this](EVENT* event)->bool
		{
		return EventSystem::Get()->HasHappened("PlayerDie");
		}, ""));
	B->AddTransition(new CSC8599::StateTransition(B->GetComponent("1"), B->GetComponent("2"), [this](EVENT* event)->bool
		{
			return true;
		}, "PetDie"));

	B->AddTransition(new CSC8599::StateTransition(B->GetComponent("1"), B->GetComponent("0"), [this](EVENT* event)->bool
		{
		return !EventSystem::Get()->HasHappened("PetDie");
		}, ""));
	B->AddTransition(new CSC8599::StateTransition(B->GetComponent("0"), B->GetComponent("2"), [this](EVENT* event)->bool
		{
			return true;
		}, "PetDie"));

	auto state0 = new  State([this](float dt)->void
		{

		});
	auto state1=new  State([this](float dt)->void
	{
		
	});

	auto state2 = new  State([this](float dt)->void
		{

		});
	auto state3 = new  State([this](float dt)->void
		{

		});

	auto C = new StateMachine("1", state1,state1);
	C->AddComponent("2", state2);
	C->AddComponent("3", state3);
	C->AddComponent("0", state0);

	C->AddTransition(new CSC8599::StateTransition(state1, state3, [this](EVENT* event)->bool
		{
			return true;
		}, "SummonDragon"));
	C->AddTransition(new CSC8599::StateTransition(state2, state1, [this](EVENT* event)->bool
		{
			return true;
		}, "DragonDie"));
	C->AddTransition(new CSC8599::StateTransition(state3, state2, [this](EVENT* event)->bool
		{
			return true;
		}, "Arrival"));

	C->AddTransition(new CSC8599::StateTransition(state2, state3, [this](EVENT* event)->bool
		{
			return true;
		}, "SummonDragon"));
	C->AddTransition(new CSC8599::StateTransition(state3, state0, [this](EVENT* event)->bool
		{
			return true;
		}, "DragonDie"));

	state1->expectation = 0;
	state2->expectation = 1;
	state3->expectation = 2;
	state0->expectation = 3;

	C->AddTransition(new CSC8599::StateTransition(state0, state3, [this](EVENT* event)->bool
		{
			return false;
		}, "",false));
	C->AddTransition(new CSC8599::StateTransition(state3, state1, [this](EVENT* event)->bool
		{
			EventSystem::Get()->PushEvent("Fix_DragonDie", 0);
			return false;
		}, "",false));
	C->AddTransition(new CSC8599::StateTransition(state1, state2, [this](EVENT* event)->bool
		{
			//没有任何意义这个回滚
			return false;
		}, "",false));


	//AddComponent("DebugA", A);
	AddComponent("DebugB", B);
	AddComponent("DebugC", C);
}

NCL::CSC8599::DebugStateMachine::~DebugStateMachine()
= default;

