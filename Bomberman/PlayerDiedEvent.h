#pragma once
#include <Event.h>

namespace bomberman
{
    class PlayerDiedEvent : public dae::Event
    {
    public:
        PlayerDiedEvent(int playerId);

        int GetPlayerId() const { return m_PlayerId; }

	private:
		int m_PlayerId;
    };
}


