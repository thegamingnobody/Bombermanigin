#include "MainGameState.h"
#include <EventManager.h>
#include "EventTypes.h"
#include <GameObject.h>
#include "StateMachineComponent.h"
#include "GameOverMenuState.h"
#include "PlayerManager.h"
#include "PlayerDiedEvent.h"
#include "ResetLevelEvent.h"
#include "LoadLevelState.h"
#include "Grid.h"

bomberman::MainGameState::MainGameState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::MainGameState::OnEnter()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	stateMachineComp.value()->SubscribeToEvent(static_cast<int>(bomberman::EventType::PLAYER_DIED));
	stateMachineComp.value()->SubscribeToEvent(static_cast<int>(bomberman::EventType::RESET_LEVEL));
}

void bomberman::MainGameState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MainGameState::Update(float /*deltaTime*/)
{
	// Check if players are dead
	auto& playerManager = bomberman::PlayerManager::GetInstance();

	if (playerManager.GetLives() < 0)
	{
		return std::make_unique<bomberman::GameOverMenuState>(*m_Owner);
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

		bool arePlayersLeft = bomberman::PlayerManager::GetInstance().SetPlayerDied(castedEvent.GetPlayerId());
		if (!arePlayersLeft)
		{
			bomberman::ResetLevelEvent newEvent;
			dae::EventManager::GetInstance().BroadcastEvent(std::make_unique<bomberman::ResetLevelEvent>(newEvent));
		}
		break;
	}
	case bomberman::EventType::RESET_LEVEL:
	{
		auto& playerManager = bomberman::PlayerManager::GetInstance();
		playerManager.DecreaseLives();
		Reset();
		break;
	}
	default:
		break;
	}
	return nullptr;
}

void bomberman::MainGameState::Reset()
{
	auto& gameManager = bomberman::GameManager::GetInstance();
	gameManager.ResetLevel();
}