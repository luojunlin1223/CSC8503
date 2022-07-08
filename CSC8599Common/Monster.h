#pragma once
#include "Player.h"
#include "StateMachine.h"
namespace NCL {
	namespace CSC8599
	{
		class Monster:public Player
		{
		public:
			Monster();
			void get_damage(const int source_id, const int damage) override;
			void get_taunt(const int source_id);
			void update(float dt) override;
		private:
			void attack_update(float dt) override;
			std::map<int,int> ThreatMap;
			StateMachine* monster_state_machine = nullptr;
			void init_state_machine();
			bool immortal = false;
			float time_stack = 0.0f;
		protected:
			bool prepare_to_attack() override;
			bool move_to_stand() override;
			bool stand_to_move() override;
			void move_update(float dt) override;
			bool alive_to_dead() override;
		};
	}
}

