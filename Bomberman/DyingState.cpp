#include "DyingState.h"
#include <iostream>
#include "EnemyManager.h"
#include <Event.h>
#include "PlayerManager.h"

bomberman::DyingState::DyingState(dae::GameObject& ownerObject, int scoreValue)
	: StateMachineBase(ownerObject)
{
	bomberman::PlayerManager::GetInstance().AddScore(scoreValue);
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::DyingState::Update(float /*deltaTime*/)
{
	//Todo: animate death of game object here
    return nullptr;
}

void bomberman::DyingState::OnEnter()
{
	bomberman::EnemyManager::GetInstance().EnemyDied();
	m_Owner->SetShouldBeRemoved();
}

void bomberman::DyingState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::DyingState::Notify(const dae::Event& /*event*/)
{
	return nullptr;
}
