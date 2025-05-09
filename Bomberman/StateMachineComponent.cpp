#include "StateMachineComponent.h"

bomberman::StateMachineComponent::StateMachineComponent(dae::GameObject& ownerObject, std::unique_ptr<StateMachineBase> startingState)
	: dae::Component(ownerObject)
	, m_CurrentState(nullptr)
{
	ChangeState(std::move(startingState));
}

void bomberman::StateMachineComponent::Update(float deltaTime)
{
	if (m_CurrentState)
	{
		m_CurrentState->Update(deltaTime);
	}
}

void bomberman::StateMachineComponent::ChangeState(std::unique_ptr<StateMachineBase> newState)
{
	if (m_CurrentState)
	{
		m_CurrentState->OnExit();
	}

	m_CurrentState = std::move(newState);

	m_CurrentState->OnEnter();
}