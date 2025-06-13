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
#include <TextureComponent.h>
#include "Grid.h"
#include "CursorMoveCommand.h"
#include "CursorSelectCommand.h"
#include <filesystem>
#include <fstream>
#include "ResourceManager.h"
#include <TextComponent.h>

bomberman::MenuState::MenuState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::MenuState::OnEnter()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();

	stateMachineComp.value()->SubscribeToEvent(static_cast<int>(bomberman::EventType::START_GAME));

	CreateMenu();
}

void bomberman::MenuState::OnExit()
{
	// Remove gamepad temporarily so that the player can correctly start using them
	dae::InputManager::GetInstance().RemoveInputDevice(dae::Action::DeviceType::Keyboard);
	dae::InputManager::GetInstance().RemoveInputDevice(dae::Action::DeviceType::Gamepad);

	auto& sceneManager = dae::SceneManager::GetInstance();
	sceneManager.RemoveScene(SCENE_MAIN_MENU);

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

void bomberman::MenuState::CreateMenu()
{
	auto& inputManager = dae::InputManager::GetInstance();
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& resourceManager = dae::ResourceManager::GetInstance();
	auto& menuScene = sceneManager.CreateScene(SCENE_MAIN_MENU);

	auto go = std::make_shared<dae::GameObject>("Title_Screen_Image", glm::vec3(0.0f, -2 * TILE_SIZE, 0.0f));
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("TitleScreen.png");
	}
	menuScene.Add(go);

	uint8_t fontSize = 32;
	auto font = resourceManager.LoadFont("PixelFont.ttf", fontSize);
	ScoreEntry highScore = GetHighScore();

	go = std::make_shared<dae::GameObject>("HighScoreEntry", glm::vec3(6.5 * TILE_SIZE, 7.75 * TILE_SIZE, 1.0f));
	go->AddComponent<dae::TextComponent>(*go.get(), highScore.first + " " + std::to_string(highScore.second), font);
	menuScene.Add(go);

	go = std::make_shared<dae::GameObject>("Cursor", glm::vec3(5 * TILE_SIZE, static_cast<int>(bomberman::CursorOptions::SinglePlayer) * TILE_SIZE, 1.0f));
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Cursor.png");
	}
	menuScene.Add(go);



	int keyboardID = inputManager.AddInputDevice(dae::Action::DeviceType::Keyboard);
	int controllerID = inputManager.AddInputDevice(dae::Action::DeviceType::Gamepad);

	inputManager.AddAction(dae::KeyboardKeys::W, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), false), keyboardID);
	inputManager.AddAction(dae::KeyboardKeys::S, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), true), keyboardID);

	inputManager.AddAction(dae::KeyboardKeys::Up, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), false), keyboardID);
	inputManager.AddAction(dae::KeyboardKeys::Down, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), true), keyboardID);

	inputManager.AddAction(dae::KeyboardKeys::Space, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorSelectCommand>(*go.get()), keyboardID);

	inputManager.AddAction(dae::GamepadButtons::DpadUp, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), false), controllerID);
	inputManager.AddAction(dae::GamepadButtons::DpadDown, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), true), controllerID);

	inputManager.AddAction(dae::GamepadButtons::FaceButtonDown, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorSelectCommand>(*go.get()), controllerID);

}

bomberman::MenuState::ScoreEntry bomberman::MenuState::GetHighScore()
{
	std::filesystem::path filePath = __FILE__;
	std::filesystem::path fileDir = filePath.parent_path();
	auto dataPath = "Scores\\Scores.txt";

	fileDir.append(dataPath);
	std::ifstream f(fileDir.c_str());

	if (!f.is_open())
	{
		f = std::ifstream(dataPath);

		if (!f.is_open())
		{
			return ScoreEntry();
		}
	}

	std::string line;
	std::string name;
	int score;

	while (f >> name >> score)
	{
		return ScoreEntry(name, score);
	}

	return ScoreEntry();
}
