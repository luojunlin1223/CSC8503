#pragma once
#include "Character.h"
namespace NCL {
	namespace CSC8599
	{
		class Player :public Character
		{
		public:
			Player();
		private:
			void move_update() override;
		public:
			void update(float dt) override;
		};
	}
}
;
