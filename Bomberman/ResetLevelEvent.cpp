#include "ResetLevelEvent.h"
#include "EventTypes.h"

bomberman::ResetLevelEvent::ResetLevelEvent()
	: Event(static_cast<int>(bomberman::EventType::RESET_LEVEL))
{
}
