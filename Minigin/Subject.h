#ifndef DAE_SUBJECT
#define DAE_SUBJECT

#include <vector>
#include "Observer.h"
#include "Event.h"

namespace dae
{
	class Subject final
	{
	public:
		Subject() = default;

		void AddObserver(Observer& observer, EventType eventType)
		{
			auto newObserver = std::make_pair(&observer, eventType);
			m_Observers.emplace_back(newObserver);
		}

		void RemoveObserver(Observer& observer)
		{
			auto it = std::find_if(m_Observers.begin(), m_Observers.end(), [&observer](const std::pair<Observer*, EventType>& pair) -> bool
				{
					return (pair.first == &observer);
				});

			if (it != m_Observers.end())
			{
				m_Observers.erase(it);
			}
		}

		void NotifyObservers(const Event& event)
		{
			for (auto& observer : m_Observers)
			{
				if (event.m_EventType != observer.second) continue;
				
				observer.first->Notify(event);
			}

		}
	private:
		std::vector<std::pair<Observer*, EventType>> m_Observers;
	};
}
#endif