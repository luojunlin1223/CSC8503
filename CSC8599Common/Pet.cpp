#include "pch.h"
#include "Pet.h"

#include "EventSystem.h"
#include "State.h"
#include "StateTransition.h"
#include "PetController.h"
#include "../CSC8503/CSC8503Common/Debug.h"
static  std::string models[3] = {"passive","assist","protect"};
Pet::Pet(Character* _owner) :owner(_owner)
{
	init_attrs("pet.json");
	user_controller_ = new PetController();
	init();
	EventSystem::Get()->RegisterEventHandler("GetDamage", [this](EVENT* p_event)->void
		{
			const int source = stoi(p_event->vArg[0]);
			const int id = stoi(p_event->vArg[1]);
			if (id == owner->GetWorldID())
			{
				switch_target(source);
				if (model == ControlModelType::PROTECT)
				{
					const auto stateMachine = get_state_machine();
					const auto actions = dynamic_cast<StateMachine*>(stateMachine->GetComponent("actions"));
					const auto attack = stateMachine->GetComponent("attack");
					actions->SetActiveComponent(attack);
				}
			}

		});
}

bool Pet::attack_to_taunt()
{
	return false;
}

void Pet::update(float dt)
{
	Character::update(dt);
	target = owner->get_target();
}

void Pet::UI_update(const Matrix4& viewMatrix, const Matrix4 projectMatrix)
{
	Character::UI_update(viewMatrix, projectMatrix);
	showHUD(viewMatrix,projectMatrix, models[static_cast<int>(model)], 9.f);
}

bool Pet::taunt_to_attack()
{
	return false;
}

bool Pet::attack_to_prepare()
{
	std::vector<std::string> arr;
	owner->get_state_machine()->GetActiveCompoentArr(arr);
	if (model == ControlModelType::PASSIVE)return user_controller_->get_inputs().buttons[STOP];
	if (model == ControlModelType::ASSIST)return std::find(arr.begin(), arr.end(), "stop") != arr.end();
	if (model == ControlModelType::PROTECT)return std::find(arr.begin(), arr.end(), "stop") != arr.end();
	return false;
}

bool Pet::prepare_to_attack()
{
	if (!target)return false;
	const auto _target = dynamic_cast<Character*>(target);
	if (!_target)return false;
	const auto health = _target->get_attr("health")._int;
	const auto input = user_controller_->get_inputs();

	std::vector<std::string> arr;
	owner->get_state_machine()->GetActiveCompoentArr(arr);

	if (model == ControlModelType::PASSIVE)return health > 0 && input.buttons[ATTACK];
	if (model == ControlModelType::ASSIST)return std::find(arr.begin(), arr.end(), "attack") != arr.end();
	if (model == ControlModelType::PROTECT)return false;
	return false;
}

bool Pet::move_to_stand()
{
	std::vector<std::string> arr;
	owner->get_state_machine()->GetActiveCompoentArr(arr);
	return std::find(arr.begin(), arr.end(), "stand") != arr.end();
}

bool Pet::stand_to_move()
{
	std::vector<std::string> arr;
	owner->get_state_machine()->GetActiveCompoentArr(arr);
	return std::find(arr.begin(),arr.end(),"move")!=arr.end();
}

void Pet::move_update()
{
	const float distance = (owner->GetTransform().GetPosition() - GetTransform().GetPosition()).Length();
	if (distance <= 10.0f)return;

	const auto origin = GetTransform().GetPosition();

	GetTransform().SetPosition(origin + (owner->GetTransform().GetPosition() - GetTransform().GetPosition()).Normalised()*1.0f);
}

void Pet::init()
{
	auto actions = (StateMachine*)state_machine_->GetComponent("actions");
	auto taunt = new State([]()->void { printf("im taunt\n"); });
	auto attack = state_machine_->GetComponent("attack");
	actions->AddComponent("taunt", taunt);
	actions->AddTransition(new StateTransition(attack, taunt, [this](EVENT* event)->bool {return attack_to_taunt(); }, ""));
	actions->AddTransition(new StateTransition(taunt, attack, [this](EVENT* event)->bool {return taunt_to_attack(); }, ""));
}
