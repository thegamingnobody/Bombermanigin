#pragma once
#include <Event.h>
#include "EventTypes.h"

namespace bomberman
{
    class StartGameEvent : public dae::Event
    {
    public:
        StartGameEvent()
            : Event(static_cast<int>(bomberman::EventType::START_GAME))
        {
        }
    };

}

