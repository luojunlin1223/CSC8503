#pragma once
#include "AbstractComponent.h"
using namespace NCL::CSC8599;
namespace NCL {
	namespace CSC8599 {
		using  StateUpdateFunction = std::function<void()>;
		class State :public AbstractComponent
		{
		public:
			explicit State(StateUpdateFunction updateFunction) {
				func = std::move(updateFunction);
			}
			~State()override = default;
			void Update(float dt) override {
				if (func != nullptr)
					func();
			}
			void GetActiveCompoentArr(std::vector<std::string>& arr) override{};
			std::string Print(int index)override;
		protected:
			StateUpdateFunction func;
		};
	}
}
