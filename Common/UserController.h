#pragma once
#include "Vector2.h"
enum Buttons
{
	ATTACK,
	STOP,
	PAUSE,
	MAX_INPUT
};

struct Input {
	bool buttons[MAX_INPUT] = { false };
	NCL::Maths::Vector2 look_direction = NCL::Maths::Vector2();
	NCL::Maths::Vector2 movement_direction = NCL::Maths::Vector2();
};

class UserController
{
public:
	UserController() = default;
	virtual ~UserController() = default;
	virtual Input get_inputs() { return {}; }
	virtual void update(const float dt) {}
};