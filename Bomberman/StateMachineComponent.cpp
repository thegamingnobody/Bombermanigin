#include "StateMachineComponent.h"

bomberman::StateMachineComponent::StateMachineComponent(dae::GameObject& ownerObject, std::unique_ptr<StateMachineBase> startingState)
	: dae::Component(ownerObject)
	, m_CurrentState(nullptr)
{
	ChangeState(std::move(startingState));
}

bomberman::StateMachineComponent::~StateMachineComponent()
{
	if (m_CurrentState)
	{
		m_CurrentState->OnExit();
	}
}

void bomberman::StateMachineComponent::Update(float deltaTime)
{
	if (m_CurrentState)
	{
		auto returnValue = m_CurrentState->Update(deltaTime);

		if (returnValue == nullptr) return;

		ChangeState(std::move(returnValue));
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