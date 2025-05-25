#include "HealthComponent.h"
#include "EventManager.h"
#include <cassert>
#include "ObjectDamagedEvent.h"
#include "BombExplodedEvent.h"
#include "EventTypes.h"
#include "StateMachineComponent.h"
#include "BrickDestroyedState.h"

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
}