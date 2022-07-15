#pragma once
#include <stack>

#include "State.h"
#include "StateMachinePlus.h"

namespace NCL
{
	namespace CSC8503
	{
		class State;
	}
}

namespace NCL {
	namespace CSC8599
	{
		class DebugStateMachine:public StateMachinePlus
		{
		public:
			DebugStateMachine();
			~DebugStateMachine() override;
			std::stack<CSC8599::State*> RePlanning();
			void RollBack(std::stack<CSC8599::State*> path);
		private:
			float time_stack=0.0f;
		};
	}
}


