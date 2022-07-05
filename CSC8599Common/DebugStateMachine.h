#pragma once
#include "StateMachinePlus.h"
namespace NCL {
	namespace CSC8599
	{
		class DebugStateMachine:public StateMachinePlus
		{
		public:
			DebugStateMachine();
			~DebugStateMachine() override;
		private:
			float time_stack=0.0f;
		};
	}
}


