#include "pch.h"
#include "PlayerController.h"
#include "../Common/Window.h"
Input NCL::CSC8599::PlayerController::get_inputs()
{
	return last_input_;
}

void NCL::CSC8599::PlayerController::update(const float dt)
{
	clear_input();
	const auto keyboard = Window::GetKeyboard();
	if (keyboard->KeyHeld(KeyboardKeys::LEFT)) last_input_.movement_direction = Vector2(-1, 0);
	if (keyboard->KeyHeld(KeyboardKeys::RIGHT)) last_input_.movement_direction = Vector2(1, 0);
	if (keyboard->KeyHeld(KeyboardKeys::UP)) last_input_.movement_direction = Vector2(0, 1);
	if (keyboard->KeyHeld(KeyboardKeys::DOWN)) last_input_.movement_direction = Vector2(0, -1);
	if (keyboard->KeyHeld(KeyboardKeys::A)) last_input_.buttons[ATTACK] = true;
	if (keyboard->KeyHeld(KeyboardKeys::S)) last_input_.buttons[STOP] = true;
	if (keyboard->KeyHeld(KeyboardKeys::TAB)) last_input_.buttons[SWITCH_TARGET] = true;
}