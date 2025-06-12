#include "NameConfirmEvent.h"
#include "EventTypes.h"

bomberman::NameConfirmEvent::NameConfirmEvent()
	: Event(static_cast<int>(bomberman::EventType::NAME_CONFIRM))
{
}
