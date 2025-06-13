#include "NameConfirmEvent.h"
#include "EventTypes.h"

bomberman::NameConfirmEvent::NameConfirmEvent(std::string name)
	: Event(static_cast<int>(bomberman::EventType::NAME_CONFIRM))
	, m_Name(name)
{
}
