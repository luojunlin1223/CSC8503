#pragma once
#include "Character.h"
namespace NCL {
	namespace CSC8599
	{
		class Monster:public Character
		{
		public:
			void get_damage(const int source_id, const int damage) override;
		private:
			void attack_update() override;
			std::map<int,int> ThreatMap;
		};
	}
}
