#include "BrickIdleState.h"
#include "BrickDestroyedState.h"
#include "EventManager.h"
#include "EventTypes.h"

bomberman::BrickIdleState::BrickIdleState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}


void bomberman::BrickIdleState::OnEnter()
{
	auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();
	if (healthComp.has_value())
	{
		m_pHealthComponent = healthComp.value();
	}
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::BrickIdleState::Update(float /*deltaTime*/)
{
	if (m_pHealthComponent != nullptr)
	{
		if (m_pHealthComponent->GetCurrentHealth() > 0) return nullptr;

		return std::make_unique<bomberman::BrickDestroyedState>(*m_Owner);
	}

	return nullptr;
}

void bomberman::BrickIdleState::OnExit()
{}

std::unique_ptr<bomberman::StateMachineBase> bomberman::BrickIdleState::Notify(const dae::Event & /*event*/)
{
	return nullptr;
}
