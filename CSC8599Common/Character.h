#pragma once
#include "../CSC8503/CSC8503Common/GameObject.h";
#include "StateMachine.h"
using namespace NCL::CSC8503;
namespace NCL {
	namespace CSC8599 {
		union data
		{
			int _int;
			float _float;
			const char* _string;
			bool _bool;
		};
		class Pet;
		class Character :public GameObject
		{
		public:
			Character();
			~Character() override = default;
			virtual void Update(float dt);
		protected:
			StateMachine* stateMachine;
			std::map<std::string, data> Attrs;
			void initAttrs();
		};
	}
}
