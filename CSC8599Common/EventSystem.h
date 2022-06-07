#pragma once
#include "AbstractStateMachine.h"
#include <list>
#include <functional>
namespace NCL
{
	namespace CSC8503
	{
		class GameWorld;
	}
}

using namespace NCL::CSC8599;
namespace NCL {
	namespace CSC8599 {
		struct EVENT;
		struct EVENT_DEFINE {
			std::string name;
			std::list<std::function<void(EVENT*)>> listFunc;
		};
		struct EVENT {
			EVENT_DEFINE* pEventDef;
			std::vector<std::string> vArg;
		};
		using EventDefContainer = std::map<std::string, EVENT_DEFINE*>;
		using EventDefContainerIterator = EventDefContainer::iterator;
		using EventQueueIterator = std::list<EVENT*>::iterator;
		class EventSystem :public AbstractStateMachine
		{
		public:
			EventSystem();
			~EventSystem() override = default;
			void Update(float dt) override;
			std::string Print(int index) override;
			void RegisterEventHandler(const std::string&, const std::function<void(EVENT*)>&);
			void PushEvent(const std::string&, int n, ...);
			static EventSystem* Get() {
				return  p_self;
			}
			static EventSystem* p_self;
			EVENT* HasHappened(const std::string&);
		private:
			void init();
			EventDefContainer eventDefContainer;
			std::list<EVENT*> eventQueue;
			std::vector<std::string> eventRecords;
		};
	}
}
