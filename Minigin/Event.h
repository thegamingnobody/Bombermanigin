#ifndef DAE_EVENT
#define DAE_EVENT

#include <string>
#include <any>
#include <iostream>
#include "EventTypes.h"


namespace dae
{
	struct Event
	{
		template<class... EventArguments>
		Event(const EventType& eventType, std::tuple<EventArguments...> arguments)
			: m_EventType(eventType)
			, m_Args(arguments)
		{
		}

		
		template<EventType T>
		typename EventArgumentMasks<T>::Args GetArgumentsAsTuple() const
		{
			using returnType = typename EventArgumentMasks<T>::Args;

			try
			{
				return std::any_cast<returnType>(m_Args);
			}
			catch (const std::bad_any_cast& e)
			{
				std::cout << "Invalid Event Arguments!\n";
				throw e;
			}
		}

		const EventType m_EventType;
	private:
		std::any m_Args;
	};
}

#endif
