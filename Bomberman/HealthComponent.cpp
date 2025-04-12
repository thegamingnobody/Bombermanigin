#include "HealthComponent.h"
#include "Event.h"
#include "EventManager.h"

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

	dae::Event event{ dae::EventType::OBJECT_DAMAGED, dae::EventArgumentMasks<dae::EventType::OBJECT_DAMAGED>::Create(owner, m_CurrentHealth) };
	dae::EventManager::GetInstance().BroadcastEvent(event);
}

void bomberman::HealthComponent::Notify(const dae::Event& event)
{
	switch (event.m_EventType)
	{
	case dae::EventType::BOMB_EXPLODED:
	{
		auto [position, radius, attackingObject] = event.GetArgumentsAsTuple<dae::EventType::BOMB_EXPLODED>();

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
