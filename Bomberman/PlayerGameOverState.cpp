#include "PlayerGameOverState.h"
#include <EventManager.h>
#include "PlayerDiedEvent.h"

bomberman::PlayerGameOverState::PlayerGameOverState(dae::GameObject& ownerObject, int playerID)
	: StateMachineBase(ownerObject)
	, m_PlayerID(playerID)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerGameOverState::Update(float /*deltaTime*/)
{
	return nullptr;
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::PlayerGameOverState::Notify(const dae::Event& /*event*/)
{
	return nullptr;
}

void bomberman::PlayerGameOverState::OnEnter()
{
}

void bomberman::PlayerGameOverState::OnExit()
{
}
