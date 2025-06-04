#include "MainGameState.h"

bomberman::MainGameState::MainGameState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::MainGameState::OnEnter()
{
}

void bomberman::MainGameState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MainGameState::Update(float /*deltaTime*/)
{
	return nullptr;
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MainGameState::Notify(const dae::Event& /*event*/)
{
	return nullptr;
}
