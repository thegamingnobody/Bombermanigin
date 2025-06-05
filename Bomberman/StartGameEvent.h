#pragma once
#include <Event.h>
#include "EventTypes.h"
#include "GameManager.h"

namespace bomberman
{
    class StartGameEvent : public dae::Event
    {
    public:
        StartGameEvent(GameMode chosenGameMode)
            : Event(static_cast<int>(bomberman::EventType::START_GAME))
			, m_ChosenGameMode(chosenGameMode)
        {
        }

        GameMode GetChosenGameMode() const
        {
            return m_ChosenGameMode;
		}

    private:
        GameMode m_ChosenGameMode;
    };

}

