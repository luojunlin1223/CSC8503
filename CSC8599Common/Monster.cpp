#include "pch.h"
#include "Monster.h"
#include "EventSystem.h"
#include "../CSC8503/CSC8503Common/GameWorld.h"
Monster::Monster()
{
	init_attrs("monster.json");
}
bool cmp_value(const std::pair<int, int> left, const std::pair<int, int> right)
{
	return left.second < right.second;
}

void Monster::get_damage(const int source_id, const int damage)
{
	Character::get_damage(source_id, damage);
	ThreatMap[source_id] += damage;
}

void Monster::get_taunt(const int source_id)
{
	ThreatMap[source_id] = 999;
}


void Monster::attack_update(float dt)
{
	const auto i = max_element(ThreatMap.begin(), ThreatMap.end(), cmp_value);
	switch_target(i->first);
	Character::attack_update(dt);
}



bool Monster::prepare_to_attack()
{
	return !ThreatMap.empty();
}

bool Monster::move_to_stand()
{
	TARGET_EXIST false;
	const float distance = (target->GetTransform().GetPosition() - GetTransform().GetPosition()).Length();
	return distance <= 10.0f;
}

bool Monster::stand_to_move()
{
	TARGET_EXIST false;
	const float distance = (target->GetTransform().GetPosition() - GetTransform().GetPosition()).Length();
	return distance > 60.0f;
}

void Monster::move_update(float dt)
{
	TARGET_EXIST;
	const auto origin = GetTransform().GetPosition();

	GetTransform().SetPosition(origin + (target->GetTransform().GetPosition() - GetTransform().GetPosition()).Normalised() * 1.0f);
}

bool Monster::alive_to_dead()
{
	const auto result = Character::alive_to_dead();
	if (result)EventSystem::Get()->PushEvent("MonsterDie", 1, std::to_string(GetWorldID()).c_str());
	return result;
}
