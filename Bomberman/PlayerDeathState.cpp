#include "PlayerDeathState.h"
#include "PlayerManager.h"
#include "HealthComponent.h"
#include "PlayerIdleState.h"
#include "GameManager.h"
#include "PlayerGameOverState.h"

bomberman::PlayerDeathState::PlayerDeathState(dae::GameObject& ownerObject, int playerID)
	: StateMachineBase(ownerObject)
	, m_PlayerID(playerID)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerDeathState::Update(float /*deltaTime*/)
{
	// animate death
	auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();

	if (!(healthComp.has_value()) or !(healthComp.value()->GetCurrentHealth() <= 0)) return nullptr;
	
	if (bomberman::PlayerManager::GetInstance().GetPlayerInfo(m_PlayerID).lives < 0)
	{
		return std::make_unique<bomberman::PlayerGameOverState>(*m_Owner, m_PlayerID);
	}

	bomberman::GameManager::GetInstance().ResetLevel();
	healthComp.value()->Heal(1);
	return std::make_unique<bomberman::PlayerIdleState>(*m_Owner, m_PlayerID);

	//return nullptr;
}

void bomberman::PlayerDeathState::OnEnter()
{
	auto& playerManager = bomberman::PlayerManager::GetInstance();
	playerManager.GetPlayerInfo(m_PlayerID).lives--;
}

void bomberman::PlayerDeathState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerDeathState::Notify(const dae::Event& /*event*/)
{
	return nullptr;
}
