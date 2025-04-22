#include "HealthComponent.h"
#include "EventManager.h"
#include <cassert>
#include "ObjectDamagedEvent.h"
#include "BombExplodedEventTest.h"
#include "EventTypes.h"

bomberman::HealthComponent::HealthComponent(dae::GameObject& ownerObject, int maxHealth, bool canSurpassMax)
	: dae::Component(ownerObject)
	, m_MaxHealth(maxHealth)
	, m_CurrentHealth(maxHealth)
	, m_CanSurpassMax(canSurpassMax)
{
}

void bomberman::HealthComponent::Heal(int amount)
{
	m_CurrentHealth += amount;
	if (!m_CanSurpassMax && m_CurrentHealth > m_MaxHealth)
	{
		m_CurrentHealth = m_MaxHealth;
	}
}

void bomberman::HealthComponent::Damage(int amount)
{
	m_CurrentHealth -= amount;
	if (m_CurrentHealth < 0)
	{
		m_CurrentHealth = 0;
	}

	auto owner = GetOwner();
	assert(owner != nullptr);

	bomberman::ObjectDamagedEvent event{ owner, m_CurrentHealth };
	dae::EventManager::GetInstance().BroadcastEvent(event);
}

void bomberman::HealthComponent::Notify(const dae::Event& event)
{
    switch (static_cast<EventType>(event.GetEventType()))
    {
    case EventType::BOMB_EXPLODED:
    {
        const auto castedEvent = dynamic_cast<const BombExplodedEventTest*>(&event);
        assert(castedEvent != nullptr); // Ensure the cast is valid

		auto position = castedEvent->GetPosition();
		auto radius = castedEvent->GetRadius();
		auto attackingObject = castedEvent->GetOwnerOfBomb();

		auto owner = GetOwner();
		assert(owner != nullptr);

		auto distanceToBomb = glm::distance(owner->GetTransform()->GetGlobalPosition(), position);

		if (distanceToBomb > radius || attackingObject == owner) break;

		Damage(1);
		break;
    }
    default:
		break;
    }
}
