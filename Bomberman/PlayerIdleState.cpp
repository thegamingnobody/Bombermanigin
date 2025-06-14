#include "PlayerIdleState.h"
#include "PlayerManager.h"
#include "HealthComponent.h"
#include "PlayerDeathState.h"
#include "HUDManager.h"
#include "PlayerGameOverState.h"

bomberman::PlayerIdleState::PlayerIdleState(dae::GameObject& ownerObject, int playerID)
	: StateMachineBase(ownerObject)
	, m_PlayerID(playerID)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerIdleState::Update(float /*deltaTime*/)
{
	auto& playerManager = bomberman::PlayerManager::GetInstance();

	if (playerManager.GetLives() < 0)
	{
		// only game over if no lives left
		return std::make_unique<bomberman::PlayerGameOverState>(*m_Owner, m_PlayerID);
	}

	auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();
	if (healthComp.has_value() and healthComp.value()->GetCurrentHealth() <= 0)
	{
		return std::make_unique<bomberman::PlayerDeathState>(*m_Owner, m_PlayerID);
	}

	return nullptr;
}

void bomberman::PlayerIdleState::OnEnter()
{
	int lives = bomberman::PlayerManager::GetInstance().GetLives();
	bomberman::HUDManager::GetInstance().SetLivesText(lives);
}

void bomberman::PlayerIdleState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerIdleState::Notify(const dae::Event& /*event*/)
{
	return nullptr;
}
