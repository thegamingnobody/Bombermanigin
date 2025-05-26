#include "PlayerDeathState.h"
#include "PlayerManager.h"
#include "HealthComponent.h"
#include "PlayerIdleState.h"
#include "GameManager.h"

bomberman::PlayerDeathState::PlayerDeathState(dae::GameObject& ownerObject, int playerID)
	: StateMachineBase(ownerObject)
	, m_PlayerID(playerID)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerDeathState::Update(float /*deltaTime*/)
{
	// animate death
	auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();
	if (healthComp.has_value() and healthComp.value()->GetCurrentHealth() <= 0)
	{
		healthComp.value()->Heal(1);
		bomberman::GameManager::GetInstance().ResetLevel();
		return std::make_unique<bomberman::PlayerIdleState>(*m_Owner, m_PlayerID);
	}

	return nullptr;
}

void bomberman::PlayerDeathState::OnEnter()
{
	auto& playerManager = bomberman::PlayerManager::GetInstance();
	playerManager.GetPlayerInfo(m_PlayerID).lives--;
}

void bomberman::PlayerDeathState::OnExit()
{
}