#include "PlayerDiedEvent.h"
#include "EventTypes.h"

bomberman::PlayerDiedEvent::PlayerDiedEvent(int playerId)
	: dae::Event(static_cast<int>(bomberman::EventType::PLAYER_DIED))
	, m_PlayerId(playerId)
{
}
