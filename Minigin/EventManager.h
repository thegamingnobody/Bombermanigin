#ifndef DAE_EVENTMANAGER
#define DAE_EVENTMANAGER

#include "Singleton.h"
#include "Observer.h"
#include <queue>
#include "Subject.h"

namespace dae
{
    class EventManager final : public Singleton<EventManager>
    {
	public:
		void Init();

		void ProcessQueue();

		void BroadcastEvent(std::unique_ptr<Event> event);

		void AddObserver(Observer& observer, int eventType);
		void RemoveObserver(Observer& observer);


	private:
		friend class Singleton<EventManager>;
		EventManager() = default;
		std::unique_ptr<Subject> m_Subject;

		//std::queue<Event> m_EventQueue;
		std::queue<std::unique_ptr<Event>> m_EventQueue;

		void ProcessEvent(std::unique_ptr<Event> event);
    };
}

#endif