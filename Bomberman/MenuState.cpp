#include "MenuState.h"
#include "StateMachineBase.h"
#include <SceneManager.h>
#include "SceneNames.h"
#include <EventManager.h>
#include "StateMachineComponent.h"
#include "EventTypes.h"
#include "LoadLevelState.h"
#include "StartGameEvent.h"
#include <InputManager.h>

bomberman::MenuState::MenuState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::MenuState::OnEnter()
{
	auto& eventManager = dae::EventManager::GetInstance();
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();

	eventManager.AddObserver(*stateMachineComp.value(), static_cast<int>(bomberman::EventType::START_GAME));
}

void bomberman::MenuState::OnExit()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	dae::EventManager::GetInstance().RemoveObserver(*stateMachineComp.value());

	// Remove gamepad temporarily so that the player can correctly start using them
	dae::InputManager::GetInstance().RemoveInputDevice(dae::Action::DeviceType::Keyboard);
	dae::InputManager::GetInstance().RemoveInputDevice(dae::Action::DeviceType::Gamepad);
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MenuState::Update(float /*deltaTime*/)
{
	return nullptr;
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MenuState::Notify(const dae::Event& event)
{
	switch (static_cast<bomberman::EventType>(event.GetEventType()))
	{
	case bomberman::EventType::START_GAME:
	{
		const auto& castedEvent = dynamic_cast<const bomberman::StartGameEvent&>(event);

		return std::make_unique<bomberman::LoadLevelState>(*m_Owner, castedEvent.GetChosenGameMode());
	}
	default:
		break;
	}

	return nullptr;
}