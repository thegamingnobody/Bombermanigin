#include "BrickDestroyedState.h"

bomberman::BrickDestroyedState::BrickDestroyedState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::BrickDestroyedState::OnEnter()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::BrickDestroyedState::Update(float /*deltaTime*/)
{
	//Todo: animate destruction
	//When animation is compolete, remove the brick
	m_Owner->SetShouldBeRemoved();
	return nullptr;
}

void bomberman::BrickDestroyedState::OnExit()
{
}