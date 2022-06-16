#pragma once
#include "Character.h"
namespace NCL {
	namespace CSC8599
	{
		class Monster:public Character
		{
		public:
			Monster();
			void get_damage(const int source_id, const int damage) override;
		private:
			void attack_update(float dt) override;
			std::map<int,int> ThreatMap;
		protected:
			bool prepare_to_attack() override;
			bool move_to_stand() override;
			bool stand_to_move() override;
			void move_update(float dt) override;
			bool alive_to_dead() override;
		};
	}
}

