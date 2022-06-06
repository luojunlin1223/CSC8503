#include "pch.h"
#include "Player.h"
#include "PlayerController.h"
NCL::CSC8599::Player::Player()
{
	user_controller_ = new PlayerController();
}

void NCL::CSC8599::Player::move_update()
{
	const auto input=user_controller_->get_inputs();
	if (input.movement_direction == Vector2(1, 0))
		physicsObject->AddForce(Vector3(100, 0, 0));
	if (input.movement_direction == Vector2(-1, 0))
		physicsObject->AddForce(Vector3(-100, 0, 0));
	if (input.movement_direction == Vector2(0, 1))
		physicsObject->AddForce(Vector3(0, 0, -100));
	if (input.movement_direction == Vector2(0, -1))
		physicsObject->AddForce(Vector3(0, 0, 100));
}

void NCL::CSC8599::Player::update(float dt)
{
	Character::update(dt);
	if (user_controller_->get_inputs().buttons[SWITCH_TARGET])
		switch_nearest_target();
}

