#include "DyingState.h"
#include <iostream>

bomberman::DyingState::DyingState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::DyingState::Update(float /*deltaTime*/)
{
	//Todo: animate death of game object
    return nullptr;
}

void bomberman::DyingState::OnEnter()
{
	std::cout << "Enemy has died\n";
	m_Owner->SetShouldBeRemoved();
}

void bomberman::DyingState::OnExit()
{
}
