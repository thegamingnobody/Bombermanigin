#include "DyingState.h"
#include <iostream>
#include "EnemyManager.h"
#include <Event.h>

bomberman::DyingState::DyingState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::DyingState::Update(float /*deltaTime*/)
{
	//Todo: animate death of game object here
    return nullptr;
}

void bomberman::DyingState::OnEnter()
{
	std::cout << "Enemy has died\n";
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
