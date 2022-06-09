#pragma once
#include "Character.h"
namespace NCL {
	namespace CSC8599
	{
		class Player :public Character
		{
		public:
			Player();
			void update(float dt) override;
		private:
			void move_update() override;
			Character* pet=nullptr;
		public:
			Character* get_pet() const
			{
				return pet;
			}

			void set_pet(Character* _pet)
			{
				this->pet = _pet;
			}
		};
	}
}
;
