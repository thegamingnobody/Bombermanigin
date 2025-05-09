#include "RoamingState.h"
#include <iostream>

bomberman::RoamingState::RoamingState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::RoamingState::Update(float /*deltaTime*/)
{
	return nullptr;
}

void bomberman::RoamingState::OnEnter()
{
}

void bomberman::RoamingState::OnExit()
{
}
