#include "MainGameState.h"
#include <EventManager.h>
#include "EventTypes.h"
#include <GameObject.h>
#include "StateMachineComponent.h"
#include "GameOverMenuState.h"
#include "PlayerManager.h"
#include "PlayerDiedEvent.h"

bomberman::MainGameState::MainGameState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::MainGameState::OnEnter()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	stateMachineComp.value()->SubscribeToEvent(static_cast<int>(bomberman::EventType::PLAYER_DIED));
}

void bomberman::MainGameState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MainGameState::Update(float /*deltaTime*/)
{
	// Check if players are dead
	auto& playerManager = bomberman::PlayerManager::GetInstance();

	if (playerManager.AreAllPlayersDead())
	{
		if (playerManager.GetPlayerInfo(0).lives <= 0)
		{
			return std::make_unique<bomberman::GameOverMenuState>(*m_Owner);
		}
		else
		{
			playerManager.ResetPlayersLifeState();
		}
	}

	return nullptr;
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MainGameState::Notify(const dae::Event& event)
{

	switch (static_cast<bomberman::EventType>(event.GetEventType()))
	{
	case bomberman::EventType::PLAYER_DIED:
	{
		auto castedEvent = static_cast<const bomberman::PlayerDiedEvent&>(event);

		bomberman::PlayerManager::GetInstance().SetPlayerDied(castedEvent.GetPlayerId());
		break;
	}
	}

	return nullptr;
}
