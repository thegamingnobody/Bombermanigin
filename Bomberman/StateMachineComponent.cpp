#include "StateMachineComponent.h"
#include <Event.h>
#include <EventManager.h>

bomberman::StateMachineComponent::StateMachineComponent(dae::GameObject& ownerObject)
	: dae::Component(ownerObject)
	, m_CurrentState(nullptr)
	, m_SubbedEventsCount(0)
{
}

bomberman::StateMachineComponent::~StateMachineComponent()
{
	auto& eventManager = dae::EventManager::GetInstance();
	for (int i = 0; i < m_SubbedEventsCount; i++)
	{
		eventManager.RemoveObserver(*this);
	}

	if (m_CurrentState)
	{
		m_CurrentState->OnExit();
	}
}

void bomberman::StateMachineComponent::Update(float deltaTime)
{
	if (!m_CurrentState) return;
		
	std::unique_ptr<StateMachineBase> returnValue = m_CurrentState->Update(deltaTime);

	if (returnValue == nullptr) return;

	ChangeState(std::move(returnValue));
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

void bomberman::StateMachineComponent::Notify(const dae::Event& event)
{
	if (!m_CurrentState) return;

	std::unique_ptr<StateMachineBase> returnValue = m_CurrentState->Notify(event);

	if (returnValue == nullptr) return;

	ChangeState(std::move(returnValue));
}

void bomberman::StateMachineComponent::SubscribeToEvent(int eventType)
{
	m_SubbedEventsCount++;
	dae::EventManager::GetInstance().AddObserver(*this, eventType);
}
