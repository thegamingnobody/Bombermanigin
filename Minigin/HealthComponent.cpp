#include "HealthComponent.h"
#include "Event.h"

dae::HealthComponent::HealthComponent(GameObject& ownerObject, int maxHealth, bool canSurpassMax)
	: Component(ownerObject)
	, m_MaxHealth(maxHealth)
	, m_CurrentHealth(maxHealth)
	, m_CanSurpassMax(canSurpassMax)
{
}

void dae::HealthComponent::Heal(int amount)
{
	m_CurrentHealth += amount;
	if (!m_CanSurpassMax && m_CurrentHealth > m_MaxHealth)
	{
		m_CurrentHealth = m_MaxHealth;
	}
}

void dae::HealthComponent::Damage(int amount)
{
	m_CurrentHealth -= amount;
	if (m_CurrentHealth < 0)
	{
		m_CurrentHealth = 0;
	}

	auto owner = GetOwner();
	assert(owner != nullptr);

	Event event{ EventType::OBJECT_DAMAGED, EventArgumentMasks<EventType::OBJECT_DAMAGED>::Create(owner, m_CurrentHealth) };
}

void dae::HealthComponent::Notify(const Event& event)
{
	switch (event.m_EventType)
	{
	case EventType::BOMB_EXPLODED:
	{
		auto [position, radius, attackingObject] = event.GetArgumentsAsTuple<EventType::BOMB_EXPLODED>();

		auto owner = GetOwner();
		assert(owner != nullptr);

		auto distanceToBomb = glm::distance(owner->GetTransform()->GetGlobalPosition(), position);

		if (distanceToBomb > radius or attackingObject == owner) break;

		Damage(1);
		break;
	}
	default:
		break;
	}
}
