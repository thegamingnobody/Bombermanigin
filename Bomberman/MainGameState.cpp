#include "MainGameState.h"
#include <EventManager.h>
#include "EventTypes.h"
#include <GameObject.h>
#include "StateMachineComponent.h"
#include "GameOverMenuState.h"


bomberman::MainGameState::MainGameState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::MainGameState::OnEnter()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	dae::EventManager::GetInstance().AddObserver(*stateMachineComp.value(), static_cast<int>(bomberman::EventType::PLAYER_DIED));
}

void bomberman::MainGameState::OnExit()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	dae::EventManager::GetInstance().RemoveObserver(*stateMachineComp.value());
	//Todo: transition to game over state + make game over screen
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MainGameState::Update(float /*deltaTime*/)
{
	return nullptr;
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MainGameState::Notify(const dae::Event& event)
{

	switch (static_cast<bomberman::EventType>(event.GetEventType()))
	{
	case bomberman::EventType::PLAYER_DIED:
	{
		//Todo: decrease player count
		//Todo: transition to game over when all players are dead
		return std::make_unique<bomberman::GameOverMenuState>(*m_Owner);
	}
	}


	return nullptr;
}
