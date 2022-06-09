#include "pch.h"
#include "EventSystem.h"
EventSystem* EventSystem::p_self = nullptr;
extern EVENT_DEFINE g_Events[] =
{
		{"test"},
		{"test2"},
		{"ThreatChanged"},
		{"GetDamage"}
};
NCL::CSC8599::EventSystem::EventSystem()
{
	p_self = this;
	init();
}

void NCL::CSC8599::EventSystem::Update(float dt)
{
	for (auto& event : eventQueue)
	{
		for (auto func = event->pEventDef->listFunc.begin(); func != event->pEventDef->listFunc.end(); func++)
		{
			(*func)(event);
		}
	}
	eventQueue.clear();
}

std::string NCL::CSC8599::EventSystem::Print(int index)
{
	std::string buffer;
	buffer += "Event Records:";
	for (auto i : eventRecords) {
		buffer += "[" + i + "]";
	}
	return buffer;
}

void NCL::CSC8599::EventSystem::RegisterEventHandler(const std::string& name, const std::function<void(EVENT*)>& func)
{
	eventDefContainer[name]->listFunc.emplace_back(func);
}

void NCL::CSC8599::EventSystem::PushEvent(const std::string& name, int n, ...)
{
	EVENT* event = new EVENT;
	event->pEventDef = eventDefContainer[name];
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; ++i) {
		event->vArg.emplace_back(va_arg(args, char*));
	}
	va_end(args);

	eventQueue.emplace_back(event);
	eventRecords.emplace_back(event->pEventDef->name);
}

EVENT* NCL::CSC8599::EventSystem::HasHappened(const std::string& name)
{
	for (auto event : eventQueue) {
		if (event->pEventDef->name == name)
			return event;
	}
	return nullptr;
}

void EventSystem::Reset()
{
	for(auto i:eventDefContainer)
	{
		i.second->listFunc.clear();
	}
	init();
}

void NCL::CSC8599::EventSystem::init()
{
	int nEventNum = sizeof(g_Events) / sizeof(EVENT_DEFINE);

	for (int i = 0; i < nEventNum; i++)
	{
		eventDefContainer[g_Events[i].name] = &(g_Events[i]);
	}
}
