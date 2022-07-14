#include "pch.h"
#include "DebugStateMachine.h"

#include <algorithm>
#include <forward_list>

#include "State.h"
#include "StateTransition.h"
#include "StateMachine.h"
#include <stack>

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
	{
	}));
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

	auto C = new StateMachine("1", state1);
	C->AddComponent("2", state2);
	C->AddComponent("3", state3);
	C->AddComponent("0", state0);

	C->AddTransition(new StateTransition(state1, state3, [this](EVENT* event)->bool
		{
			return true;
		}, "SummonDragon"));
	C->AddTransition(new StateTransition(state2, state1, [this](EVENT* event)->bool
		{
			return true;
		}, "DragonDie"));
	C->AddTransition(new StateTransition(state3, state2, [this](EVENT* event)->bool
		{
			return true;
		}, "Arrival"));

	C->AddTransition(new StateTransition(state2, state3, [this](EVENT* event)->bool
		{
			return true;
		}, "SummonDragon"));
	C->AddTransition(new StateTransition(state3, state0, [this](EVENT* event)->bool
		{
			EventSystem::Get()->PushEvent("Debug_DragonDie",0);
			return true;
		}, "DragonDie"));

	state1->expectation = 0;
	state2->expectation = 1;
	state3->expectation = 2;
	state0->expectation = 3;


	C->AddTransition(new StateTransition(state0, state3, [this](EVENT* event)->bool
		{
			return false;
		}, ""));
	C->AddTransition(new StateTransition(state3, state1, [this](EVENT* event)->bool
		{
			return false;
		}, ""));
	C->AddTransition(new StateTransition(state1, state2, [this](EVENT* event)->bool
		{
			return false;
		}, ""));


	//AddComponent("DebugA", A);
	AddComponent("DebugB", B);
	AddComponent("DebugC", C);
}

NCL::CSC8599::DebugStateMachine::~DebugStateMachine()
= default;

struct Node
{
	Node(State* state) :
		f(0), g(0), h(0), state(state)
	{}
	int f, g, h;
	State* state;
};

bool cmp(const std::forward_list<Node> a, const std::forward_list<Node> b) {
	return a.begin()->f < b.begin()->f;
}
void NCL::CSC8599::DebugStateMachine::RePlanning()
{
	auto state_machine = dynamic_cast<StateMachine*>(GetComponent("DebugC"));
	auto source = dynamic_cast<State*>(state_machine->GetComponent("0"));
	auto destination = dynamic_cast<State*>(state_machine->GetComponent("1"));


	std::vector<std::forward_list<Node>> openList;
	std::vector<std::forward_list<Node>> closeList;

	auto allState = state_machine->get_component_container();
	auto allTrans = state_machine->get_all_transitions();
	auto node = Node(source);
	auto temp = std::forward_list<Node>();
	temp.push_front(node);
	openList.emplace_back(temp);

	while (!openList.empty())
	{
		std::sort(openList.begin(),openList.end(),cmp);
		auto currentNode = openList[0];
		openList.erase(openList.begin());
		closeList.emplace_back(currentNode);
		if (currentNode.begin()->state == destination)
		{
			int i = 0;
		}
		std::pair<TransitionIterator, TransitionIterator> range = allTrans.equal_range(currentNode.begin()->state);
		auto currentG = currentNode.begin()->g;
		for (auto& i = range.first; i != range.second; ++i)
		{
			
			bool skip = false;
			auto newState = dynamic_cast<State*>(i->second->GetDestinationState());
			auto newNode = Node(newState);
			for (auto i : closeList)
			{
				if (newNode.state == i.begin()->state)
				{
					skip=true;
					break;
				}
			}
			if(skip)continue;
			newNode.g = currentG + 1;
		/*	newNode.h = -(newNode.state->expectation- destination->expectation);
			newNode.f = newNode.g + newNode.h;*/
			newNode.f = newNode.g;

			std::forward_list<Node> currentNodeCopy(currentNode);
			currentNodeCopy.emplace_front( newNode);
			openList.emplace_back(currentNodeCopy);
		}
	}
}