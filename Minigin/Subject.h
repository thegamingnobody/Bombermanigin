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

		void AddObserver(Observer& observer, int eventType)
		{
			auto newObserver = std::make_pair(&observer, eventType);
			m_Observers.emplace_back(newObserver);
		}

		void RemoveObserver(Observer& observer)
		{
			auto it = std::find_if(m_Observers.begin(), m_Observers.end(), [&observer](const std::pair<Observer*, int>& pair) -> bool
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
				if (event.GetEventType() != observer.second) continue;
				
				observer.first->Notify(event);
			}

		}
	private:
		std::vector<std::pair<Observer*, int>> m_Observers;
	};
}
#endif