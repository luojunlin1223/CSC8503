#pragma once
#include "../CSC8503/CSC8503Common/GameObject.h";
#include "../Common/UserController.h"
#include "StateMachine.h"
using namespace NCL::CSC8503;
namespace NCL {
	namespace CSC8599 {
		union data
		{
			int _int;
			float _float;
			const char* _string;
			bool _bool;
		};
		class Pet;
		class Character :public GameObject
		{
		public:
			Character();
			~Character() override = default;
			void update(float dt) override;
			data get_attr(const std::string& attr_name);
			void set_attr(const std::string& attr_name, const data data);
			std::string print();
			virtual void get_damage(const int source_id,const int damage);
			bool switch_target(const int target_id);
			bool switch_nearest_target();
		protected:
			StateMachine* state_machine_;
			std::map<std::string, data> attrs_;
			UserController* user_controller_;
			GameObject* target;
			GameObject* pet;
			void init_attrs();
			void init_state_machine();

			virtual bool attack_to_prepare();
			virtual bool prepare_to_attack();
			virtual bool alive_to_dead();
			virtual bool move_to_stand();
			virtual bool stand_to_move();

			virtual void attack_update();
			virtual void prepare_update();
			virtual void move_update();
			virtual void stand_update();
			virtual void dead_update();
		};
	}
}
