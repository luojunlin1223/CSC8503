#pragma once
#include "State.h"
#include "StateMachine.h"
#include "StateMachinePlus.h"
namespace NCL {
	namespace CSC8599
	{
		class DebugStateMachine:public StateMachinePlus
		{
		public:
			DebugStateMachine();
			~DebugStateMachine() override;
			void RePlanning();
		private:
			float time_stack=0.0f;
		};
	}
}


