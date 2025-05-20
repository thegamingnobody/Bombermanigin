#include "BrickIdleState.h"
#include "HealthComponent.h"
#include "BrickDestroyedState.h"

bomberman::BrickIdleState::BrickIdleState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}


void bomberman::BrickIdleState::OnEnter()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::BrickIdleState::Update(float /*deltaTime*/)
{
	auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();
	if (healthComp.has_value())
	{
		if (healthComp.value()->GetCurrentHealth() >= 0) return nullptr;

		return std::make_unique<bomberman::BrickDestroyedState>(*m_Owner);
	}

	return nullptr;
}

void bomberman::BrickIdleState::OnExit()
{
}