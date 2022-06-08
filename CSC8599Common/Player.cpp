#include "pch.h"
#include "Player.h"
#include "PlayerController.h"
#include "../CSC8503/CSC8503Common/Debug.h"
NCL::CSC8599::Player::Player()
{
	user_controller_ = new PlayerController();
}

void NCL::CSC8599::Player::move_update()
{
	const auto input=user_controller_->get_inputs();
	const auto origin = GetTransform().GetPosition();
	if (input.movement_direction == Vector2(1, 0))
		GetTransform().SetPosition(origin+Vector3(1, 0, 0));
	if (input.movement_direction == Vector2(-1, 0))
		GetTransform().SetPosition(origin + Vector3(-1, 0, 0));
	if (input.movement_direction == Vector2(0, 1))
		GetTransform().SetPosition(origin + Vector3(0, 0, -1));
	if (input.movement_direction == Vector2(0, -1))
		GetTransform().SetPosition(origin + Vector3(0, 0, 1));
}


void NCL::CSC8599::Player::update(float dt)
{
	Character::update(dt);
	if (user_controller_->get_inputs().buttons[SWITCH_TARGET])
		switch_nearest_target();
}

