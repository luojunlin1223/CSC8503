#pragma once
#include "StateMachine.h"
namespace NCL {
	namespace CSC8599
	{
		class DebugStateMachine:public StateMachine
		{
		public:
			DebugStateMachine();
			~DebugStateMachine() override;
		private:
			float time_stack=0.0f;
		};
	}
}


