#pragma once
#include "AbstractComponent.h"
using namespace NCL::CSC8599;
namespace NCL {
	namespace CSC8599 {
		using  StateUpdateFunction = std::function<void(float)>;
		class State :public AbstractComponent
		{
		public:
			explicit State(StateUpdateFunction updateFunction) {
				func = std::move(updateFunction);
			}
			~State()override = default;
			void Update(float dt) override {
				if (func != nullptr)
					func(dt);
			}
			std::string Print(int index)override;
		protected:
			StateUpdateFunction func;
		};
	}
}
