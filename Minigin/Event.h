#ifndef DAE_EVENT
#define DAE_EVENT

#include <string>
#include <any>
#include <iostream>

namespace dae
{
	enum class EventType
	{
		BOMB_EXPLODED
	};

	struct Event
	{
		template<class... EventArguments>
		Event(const EventType& eventType, std::tuple<EventArguments...> arguments, int const playerNumber)
			: m_EventType(eventType)
			, m_Args(arguments)
			//, m_playerNumber(playerNumber)
		{
		}

		template<class... EventArguments>
		std::tuple<EventArguments...> GetArgumentsAsTuple() const
		{
			try
			{
				return std::any_cast<std::tuple<EventArguments...>>(m_Args);
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
