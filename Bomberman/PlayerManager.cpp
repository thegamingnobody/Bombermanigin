#include "PlayerManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "Grid.h"
#include <TextureComponent.h>
#include "HealthComponent.h"
#include "BoxCollider.h"
#include "MoveCommand.h"
#include "AttackCommand.h"
#include "SkipLevelCommand.h"
#include "ResetLevelCommand.h"
#include "StateMachineComponent.h"
#include "PlayerIdleState.h"
#include "SceneNames.h"
#include "HUDManager.h"

void bomberman::PlayerManager::CreatePlayer(dae::Action::DeviceType deviceType, InputMapping inputMapping)
{
	auto& inputManager = dae::InputManager::GetInstance();
	auto& camera = dae::Camera::GetInstance();
	auto& grid = bomberman::Grid::GetInstance();
	auto& sceneManager = dae::SceneManager::GetInstance();

	float tileScale{ camera.GetWindowScale() };
	//float const playerSpeed{ 75.0f };
	float const playerSpeed{ 150.0f };
	auto playerScene = sceneManager.GetScene(SCENE_PLAYERS);

	int const playerInputID = inputManager.AddInputDevice(deviceType);

	PlayerInfo playerInfo;

	playerInfo.playerID = static_cast<int>(m_Players.size());
	playerInfo.name = "Player " + std::to_string(playerInfo.playerID + 1);
	playerInfo.deviceType = deviceType;

	m_Players.emplace_back(playerInfo);
	
	bomberman::GridCell playerStartCell{ 1, 1 };
	auto go = std::make_shared<dae::GameObject>(playerInfo.name, grid.GridCoordToWorldPos(playerStartCell), playerInputID);
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Bomberman_S_1.png");
		auto textureSize = textureComponent.GetSize();
	
		auto& stateMachineComponent = go->AddComponent<bomberman::StateMachineComponent>(*go.get());
		auto idleState = std::make_unique<bomberman::PlayerIdleState>(*go.get(), playerInfo.playerID);
		stateMachineComponent.ChangeState(std::move(idleState));

		go->AddComponent<bomberman::HealthComponent>(*go.get(), 1);
		go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Player, bomberman::Box(4 * tileScale, 1 * tileScale, 8 * tileScale, 14 * tileScale));
	}
	playerScene->Add(go);
	camera.SetTrackingTarget(*go.get());

	auto upCommand = std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, -1.0f, 0.0f) * playerSpeed);
	auto downCommand = std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, 1.0f, 0.0f) * playerSpeed);
	auto leftCommand = std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(-1.0f, 0.0f, 0.0f) * playerSpeed);
	auto rightCommand = std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(1.0f, 0.0f, 0.0f) * playerSpeed);
	auto attackCommand = std::make_shared<bomberman::AttackCommand>(*go.get(), playerInfo.playerID);

	switch (inputMapping.deviceType)
	{
	case dae::Action::DeviceType::Keyboard:
		inputManager.AddAction(dae::KeyboardKeys(inputMapping.upButton),	dae::InputType::Held, upCommand,	playerInputID);
		inputManager.AddAction(dae::KeyboardKeys(inputMapping.downButton),	dae::InputType::Held, downCommand,	playerInputID);
		inputManager.AddAction(dae::KeyboardKeys(inputMapping.leftButton),	dae::InputType::Held, leftCommand,	playerInputID);
		inputManager.AddAction(dae::KeyboardKeys(inputMapping.rightButton), dae::InputType::Held, rightCommand, playerInputID);
		inputManager.AddAction(dae::KeyboardKeys(inputMapping.bombButton),	dae::InputType::PressedThisFrame, attackCommand, playerInputID);

		inputManager.AddAction(dae::KeyboardKeys::F1, dae::InputType::PressedThisFrame, std::make_shared<bomberman::SkipLevelCommand>(), playerInputID);
		inputManager.AddAction(dae::KeyboardKeys::R, dae::InputType::PressedThisFrame, std::make_shared<bomberman::ResetLevelCommand>(), playerInputID);
		break;
	case dae::Action::DeviceType::Gamepad:
		inputManager.AddAction(dae::GamepadButtons(inputMapping.upButton),		dae::InputType::Held, upCommand,	playerInputID);
		inputManager.AddAction(dae::GamepadButtons(inputMapping.downButton),	dae::InputType::Held, downCommand,	playerInputID);
		inputManager.AddAction(dae::GamepadButtons(inputMapping.leftButton),	dae::InputType::Held, leftCommand,	playerInputID);
		inputManager.AddAction(dae::GamepadButtons(inputMapping.rightButton),	dae::InputType::Held, rightCommand, playerInputID);
		inputManager.AddAction(dae::GamepadButtons(inputMapping.bombButton),	dae::InputType::PressedThisFrame, attackCommand, playerInputID);
		break;
	}

}

void bomberman::PlayerManager::RemovePlayer(int playerID)
{
	if (playerID < 0 || playerID >= static_cast<int>(m_Players.size()))
	{
		throw std::out_of_range("Player ID is out of range.");
	}

	m_Players[playerID] = PlayerInfo{}; // Reset player info
}

void bomberman::PlayerManager::ClearPlayers()
{
	m_Players.clear();
}

void bomberman::PlayerManager::ResetScore()
{
	m_Score = 0;

	bomberman::HUDManager::GetInstance().SetScoreText(m_Score);
}

void bomberman::PlayerManager::AddScore(int score)
{
	m_Score += score;

	bomberman::HUDManager::GetInstance().SetScoreText(m_Score);
}