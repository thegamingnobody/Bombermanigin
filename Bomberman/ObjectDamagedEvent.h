#ifndef OBJECTDAMAGEDEVENT
#define OBJECTDAMAGEDEVENT

#include <Event.h>
#include <GameObject.h>
#include "EventTypes.h"

namespace bomberman
{
    class ObjectDamagedEvent : public dae::Event
    {
    public:
		ObjectDamagedEvent(dae::GameObject* damagedObject, int newHealth)
			: Event(static_cast<int>(EventType::OBJECT_DAMAGED))
			, m_DamagedObject(damagedObject)
			, m_NewHealth(newHealth)
		{
		}

		dae::GameObject* GetDamagedObject() const { return m_DamagedObject; }

		int GetNewHealth() const { return m_NewHealth; }

    private:
		dae::GameObject* m_DamagedObject;
		int m_NewHealth;
    };
}

#endif