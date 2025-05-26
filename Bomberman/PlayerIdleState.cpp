#include "PlayerIdleState.h"
#include "PlayerManager.h"
#include "HealthComponent.h"
#include "PlayerDeathState.h"

bomberman::PlayerIdleState::PlayerIdleState(dae::GameObject& ownerObject, int playerID)
	: StateMachineBase(ownerObject)
	, m_PlayerID(playerID)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerIdleState::Update(float /*deltaTime*/)
{
	auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();
	if (healthComp.has_value() and healthComp.value()->GetCurrentHealth() <= 0)
	{
		return std::make_unique<bomberman::PlayerDeathState>(*m_Owner, m_PlayerID);
	}

	return nullptr;
}

void bomberman::PlayerIdleState::OnEnter()
{
}

void bomberman::PlayerIdleState::OnExit()
{
}