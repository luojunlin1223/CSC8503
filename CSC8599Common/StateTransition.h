#pragma once
#include "AbstractComponent.h"
#include "EventSystem.h"
using namespace NCL::CSC8599;
namespace NCL {
	namespace CSC8599 {
		class State;
		using StateTransitionFunction = std::function<bool(EVENT*)>;
		class StateTransition {
		public:
			StateTransition(AbstractComponent* source, AbstractComponent* dest, StateTransitionFunction f, std::string _trigger,
				bool rollback=false) {
				sourceState = source;
				destinationState = dest;
				transitionFunction = std::move(f);
				trigger = _trigger;
				rollBack = rollback;
			}
			bool CanTransition() const;
			void RollBack();
			AbstractComponent* GetDestinationState() const { return destinationState; }
			AbstractComponent* GetSourceState() const { return sourceState; }
			bool rollBack;
		protected:
			AbstractComponent* sourceState;
			AbstractComponent* destinationState;
			StateTransitionFunction transitionFunction;
			std::string trigger;
		};
	}
}
