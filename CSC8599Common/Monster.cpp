#include "pch.h"
#include "Monster.h"
#include "EventSystem.h"
#include "../CSC8503/CSC8503Common/GameWorld.h"
bool cmp_value(const std::pair<int, int> left, const std::pair<int, int> right)
{
	return left.second < right.second;
}


void Monster::get_damage(const int source_id, const int damage)
{
	Character::get_damage(source_id, damage);
	ThreatMap[source_id] += 1;
}


void Monster::attack_update()
{
	const auto i = max_element(ThreatMap.begin(), ThreatMap.end(), cmp_value);
	if (!switch_target(i->first))return;
	EventSystem::Get()->PushEvent("ThreatChanged", 1, std::to_string(target->GetWorldID()).c_str());
	Character::attack_update();
}
