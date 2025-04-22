#ifndef DAE_EVENT
#define DAE_EVENT

#include <string>
#include <any>
#include <iostream>

namespace dae
{
	class Event
	{
	public:
		Event(const int eventType)	
			: m_EventType(eventType)
		{
		}

		virtual ~Event() = default;

		int GetEventType() const
		{
			return m_EventType;
		}

	private:
		const int m_EventType;
	};
}

#endif
