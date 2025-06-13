#include "PlayerDeathState.h"
#include "PlayerManager.h"
#include "HealthComponent.h"
#include "PlayerIdleState.h"
#include "GameManager.h"
#include "PlayerGameOverState.h"
#include "PlayerDiedEvent.h"
#include "EventManager.h"
#include "EventTypes.h"
#include "StateMachineComponent.h"
#include "ResetLevelEvent.h"

bomberman::PlayerDeathState::PlayerDeathState(dae::GameObject& ownerObject, int playerID)
	: StateMachineBase(ownerObject)
	, m_PlayerID(playerID)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerDeathState::Update(float /*deltaTime*/)
{
	auto& playerManager = bomberman::PlayerManager::GetInstance();

	if (playerManager.GetPlayerInfo(m_PlayerID).isAlive)
	{
		// Only triggers once
		bomberman::PlayerDiedEvent event(m_PlayerID);
		dae::EventManager::GetInstance().BroadcastEvent(std::make_unique<PlayerDiedEvent>(event));
		m_Owner->SetIsHidden(true);
	}

	bool playersLeft = playerManager.SetPlayerDied(m_PlayerID);

	if (playersLeft) return nullptr;

	// no players left, reset

	if (playerManager.GetPlayerInfo(m_PlayerID).lives < 0)
	{
		// only game over if no lives left
		return std::make_unique<bomberman::PlayerGameOverState>(*m_Owner, m_PlayerID);
	}

	bomberman::GameManager::GetInstance().ResetLevel();
	dae::EventManager::GetInstance().BroadcastEvent(std::make_unique<ResetLevelEvent>());
	return nullptr;
}

void bomberman::PlayerDeathState::OnEnter()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	stateMachineComp.value()->SubscribeToEvent(static_cast<int>(bomberman::EventType::RESET_LEVEL));
}

void bomberman::PlayerDeathState::OnExit()
{
	m_Owner->SetIsHidden(false);
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerDeathState::Notify(const dae::Event& event)
{
	switch (static_cast<bomberman::EventType>(event.GetEventType()))
	{
	case bomberman::EventType::RESET_LEVEL:
	{
		auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();
		if (healthComp.has_value())
		{
			healthComp.value()->Heal(1);
		}
		return std::make_unique<bomberman::PlayerIdleState>(*m_Owner, m_PlayerID);
	}
	default:
		break;
	}

	return nullptr;
}
