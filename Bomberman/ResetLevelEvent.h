#pragma once
#include <Event.h>

namespace bomberman
{
    class ResetLevelEvent : public dae::Event
    {
    public:
        ResetLevelEvent();
        ~ResetLevelEvent() = default;
        
    };
}


