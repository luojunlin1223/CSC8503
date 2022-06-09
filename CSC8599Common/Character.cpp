#include "pch.h"
#include "Character.h"
#include "../Common/Assets.h"
#include "State.h"
#include "StateMachinePlus.h"
#include "EventSystem.h"
#include "StateTransition.h"
#include "../CSC8503/CSC8503Common/GameWorld.h"

NCL::CSC8599::Character::Character()
	:state_machine_(nullptr),
	target(nullptr),
	user_controller_(new UserController())
{
	init_state_machine();
}
void NCL::CSC8599::Character::update(float dt)
{
	user_controller_->update(dt);
	state_machine_->Update(dt);
}
std::string NCL::CSC8599::Character::print()
{
	std::string buffer;
	buffer += "Health:" + std::to_string(get_attr("health")._int) + "\n";
	buffer += state_machine_->Print(0);
	return buffer;
}

void NCL::CSC8599::Character::get_damage(const int source_id,const int damage)
{
	auto data = get_attr("health");
	auto health = data._int;
	health = health - damage <= 0 ? 0 : health - damage;
	data._int = health;
	set_attr("health", data);
	EventSystem::Get()->PushEvent("GetDamage", 2, std::to_string(source_id).c_str(),std::to_string(worldID).c_str());
}

bool Character::switch_target(const int target_id)
{
	const auto temp=GameWorld::Get()->find_game_object(target_id);
	if (!temp)return false;
	if (target && target->GetWorldID() == temp->GetWorldID())return false;
	target = temp;
	EventSystem::Get()->PushEvent("ThreatChanged", 1, std::to_string(target->GetWorldID()).c_str());
	return true;
}

bool Character::switch_nearest_target()
{
	const auto temp = GameWorld::Get()->find_nearest_game_object(this);
	if (!temp)return false;
	target = temp;
	EventSystem::Get()->PushEvent("ThreatChanged", 1, std::to_string(target->GetWorldID()).c_str());
	return true;
}

void Character::UI_update(const Matrix4& viewMatrix, const Matrix4 projectMatrix)
{
	showHUD(viewMatrix,projectMatrix,name,5.f);
	showHUD(viewMatrix, projectMatrix, std::to_string(get_attr("health")._int), 7.f);
}

data NCL::CSC8599::Character::get_attr(const std::string& attr_name)
{
	const auto result = attrs_.find(attr_name);
	if (result == attrs_.end())return { 0 };
	return result->second;
}
void NCL::CSC8599::Character::set_attr(const std::string& attr_name,const data data)
{
	attrs_[attr_name] = data;
}
void NCL::CSC8599::Character::init_attrs(const std::string& attr_file_name)
{
	std::ifstream t(NCL::Assets::DATADIR + attr_file_name);
	rapidjson::IStreamWrapper isw(t);
	rapidjson::Document document;
	document.ParseStream(isw);
	for (rapidjson::Document::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	{
		if (itr->value.IsString())
		{
			const char* source = itr->value.GetString();
			name = std::string(source);
		}
		if (itr->value.IsInt())attrs_[itr->name.GetString()]._int = itr->value.GetInt();
		if (itr->value.IsFloat())attrs_[itr->name.GetString()]._float = itr->value.GetFloat();
		if (itr->value.IsBool())attrs_[itr->name.GetString()]._bool = itr->value.GetBool();
	}
}

void NCL::CSC8599::Character::init_state_machine()
{
	const auto attack = new State([this]()->void { return attack_update(); });
	const auto prepare = new State([this]()->void { return prepare_update(); });

	const auto actions = new StateMachine("prepare", prepare);
	actions->AddComponent("attack", attack);
	actions->AddTransition(new StateTransition(attack, prepare,
		[this](EVENT* event)->bool {return attack_to_prepare(); }, ""));
	actions->AddTransition(new StateTransition(prepare, attack,
		[this](EVENT* event)->bool {return prepare_to_attack(); }, ""));

	const auto move = new State([this]()->void { return move_update(); });
	const auto stand = new State([this]()->void { return stand_update(); });
	const auto movement = new StateMachine("stand", stand);
	movement->AddComponent("move", move);
	movement->AddTransition(new StateTransition(move, stand,
		[this](EVENT* event)->bool {return move_to_stand(); }, ""));
	movement->AddTransition(new StateTransition(stand, move,
		[this](EVENT* event)->bool {return stand_to_move(); }, ""));

	const auto model = new StateMachinePlus;
	model->AddComponent("actions", actions);
	model->AddComponent("movement", movement);

	const auto alive = new StateMachinePlus;
	alive->AddComponent("model", model);

	const auto dead = new State([this]()->void { return dead_update(); });
	state_machine_ = new StateMachine("alive", alive);
	state_machine_->AddComponent("dead", dead);
	state_machine_->AddTransition(new StateTransition(alive, dead,
		[this](EVENT* event)->bool {return alive_to_dead(); }, ""));
}

void Character::showHUD(const Matrix4& viewMatrix, const Matrix4 projectMatrix, std::string text, const float height)
{
	Matrix4 local = GetTransform().GetMatrix();
	local.SetPositionVector({ 0, 0, 0 });

	Vector3 up = local * Vector4(0, 1, 0, 1.0f);
	Vector3 left = local * Vector4(-1, 0, 0, 1.0f);
	Vector3 worldPos = GetTransform().GetPosition();
	Vector3 clip = projectMatrix * viewMatrix * worldPos;
	Vector2 canvas;
	canvas.x = (clip.x + 2.0f) * 25.0f - 1.0f * text.size();
	canvas.y = (1.0f - clip.y) * 50.0f - height;
	Debug::Print(text, canvas);
}

bool NCL::CSC8599::Character::attack_to_prepare()
{
	if (!target)return false;
	const auto _target = dynamic_cast<Character*>(target);
	if (!_target)return false;
	const auto health = _target->get_attr("health")._int;
	const auto input = user_controller_->get_inputs();
	return health == 0 || input.buttons[STOP];
}

bool Character::prepare_to_attack()
{
	if (!target)return false;
	const auto _target = dynamic_cast<Character*>(target);
	if (!_target)return false;
	const auto health = _target->get_attr("health")._int;
	const auto input = user_controller_->get_inputs();
	return health != 0 && input.buttons[ATTACK];
}

bool Character::alive_to_dead()
{
	return get_attr("health")._int==0;
}

bool Character::move_to_stand()
{
	return user_controller_->get_inputs().buttons[STOP];
}

bool Character::stand_to_move()
{
	return user_controller_->get_inputs().movement_direction!=Vector2();
}

void Character::attack_update()
{
	if (!target)return;
	const auto _target = dynamic_cast<Character*>(target);
	if (!_target)return;
	_target->get_damage(GetWorldID(),1);
}

void Character::prepare_update()
{
}

void Character::move_update()
{
}

void Character::stand_update()
{
}

void Character::dead_update()
{
}