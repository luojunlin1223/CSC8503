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
			void move_update(float dt) override;
			Character* pet=nullptr;
		protected:
			bool alive_to_dead() override;
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
