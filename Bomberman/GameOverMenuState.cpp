#include "GameOverMenuState.h"
#include <SceneManager.h>
#include <InputManager.h>
#include "SceneNames.h"
#include <GameObject.h>
#include <Scene.h>
#include "Grid.h"
#include <TextComponent.h>
#include <ResourceManager.h>
#include "IncDecLetterCommand.h"
#include "NameEntryManager.h"
#include "SwitchLetterIndex.h"
#include <SDL_ttf.h>
#include "StateMachineComponent.h"
#include "EventTypes.h"
#include <EventManager.h>
#include "MenuState.h"
#include "NameConfirmCommand.h"
#include "PlayerManager.h"


bomberman::GameOverMenuState::GameOverMenuState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::GameOverMenuState::OnEnter()
{
	auto& sceneManager = dae::SceneManager::GetInstance();

	sceneManager.RemoveScene(SCENE_MAP);
	sceneManager.RemoveScene(SCENE_OBJECTS);
	sceneManager.RemoveScene(SCENE_PLAYERS);
	sceneManager.RemoveScene(SCENE_HUD);

	CreateGameOverScreen();

	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	dae::EventManager::GetInstance().AddObserver(*stateMachineComp.value(), static_cast<int>(bomberman::EventType::NAME_CONFIRM));

}

void bomberman::GameOverMenuState::OnExit()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	dae::EventManager::GetInstance().RemoveObserver(*stateMachineComp.value());

	dae::Camera::GetInstance().EnableTracking(false);

	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.RemoveInputDevice(dae::Action::DeviceType::Keyboard);
	inputManager.RemoveInputDevice(dae::Action::DeviceType::Gamepad);

	sceneManager.RemoveScene(SCENE_GAMEOVER);

	auto& playerManager = bomberman::PlayerManager::GetInstance();
	playerManager.ClearPlayers();
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::GameOverMenuState::Update(float /*deltaTime*/)
{
	return nullptr;
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::GameOverMenuState::Notify(const dae::Event& event)
{ 
	switch (static_cast<bomberman::EventType>(event.GetEventType()))
	{
	case bomberman::EventType::NAME_CONFIRM:
	{
		//Todo: save name & score to file
		//auto& nameEntryManager = bomberman::NameEntryManager::GetInstance();
		//nameEntryManager.SaveName();
		return std::make_unique<bomberman::MenuState>(*m_Owner);
	}
	default:
		break;
	}

	return nullptr;
}

void bomberman::GameOverMenuState::CreateGameOverScreen()
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& inputManager = dae::InputManager::GetInstance();
	auto& resourceManager = dae::ResourceManager::GetInstance();

	auto& gameOverScene = sceneManager.CreateScene(SCENE_GAMEOVER);

	auto go = std::make_shared<dae::GameObject>("TrackingTargetReset");
	gameOverScene.Add(go);
	dae::Camera::GetInstance().SetTrackingTarget(*go.get());

	uint8_t fontSize = 32;
	auto font = resourceManager.LoadFont("PixelFont.ttf", fontSize);
	auto test = font->GetFont();
	int width;
	int height;
	TTF_SizeText(test, "A", &width, &height);

	auto middleLetterX = TILE_SIZE * 7;

	go = std::make_shared<dae::GameObject>("Letter0", glm::vec3(middleLetterX - width, TILE_SIZE * 4, 0.0f));
	go->AddComponent<dae::TextComponent>(*go.get(), "A", font);
	gameOverScene.Add(go);
	go = std::make_shared<dae::GameObject>("Letter1", glm::vec3(middleLetterX, TILE_SIZE * 4, 0.0f));
	go->AddComponent<dae::TextComponent>(*go.get(), "A", font);
	gameOverScene.Add(go);
	go = std::make_shared<dae::GameObject>("Letter2", glm::vec3(middleLetterX + width, TILE_SIZE * 4, 0.0f));
	go->AddComponent<dae::TextComponent>(*go.get(), "A", font);
	gameOverScene.Add(go);

	go = std::make_shared<dae::GameObject>("Cursor", glm::vec3(middleLetterX - width, TILE_SIZE * 3, 0.0f));
	go->AddComponent<dae::TextComponent>(*go.get(), "V  ", font);
	gameOverScene.Add(go);

	inputManager.RemoveInputDevice(dae::Action::DeviceType::Keyboard);
	inputManager.RemoveInputDevice(dae::Action::DeviceType::Gamepad);

	int keyboardID = inputManager.AddInputDevice(dae::Action::DeviceType::Keyboard);

	inputManager.AddAction(dae::KeyboardKeys::W, dae::InputType::PressedThisFrame, std::make_shared<IncDecLetterCommand>(true), keyboardID);
	inputManager.AddAction(dae::KeyboardKeys::S, dae::InputType::PressedThisFrame, std::make_shared<IncDecLetterCommand>(false), keyboardID);

	inputManager.AddAction(dae::KeyboardKeys::A, dae::InputType::PressedThisFrame, std::make_shared<SwitchLetterIndex>(false), keyboardID);
	inputManager.AddAction(dae::KeyboardKeys::D, dae::InputType::PressedThisFrame, std::make_shared<SwitchLetterIndex>(true), keyboardID);
	
	inputManager.AddAction(dae::KeyboardKeys::Enter, dae::InputType::PressedThisFrame, std::make_shared<bomberman::NameConfirmCommand>(), keyboardID);

	bomberman::NameEntryManager::GetInstance().Reset();
}
