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

void bomberman::PlayerManager::CreatePlayer(InputMapping mapping1, InputMapping mapping2)
{
	auto& camera = dae::Camera::GetInstance();
	auto& sceneManager = dae::SceneManager::GetInstance();

	auto playerScene = sceneManager.GetScene(SCENE_PLAYERS);

	PlayerInfo playerInfo = CreatePlayerInfo(mapping1, mapping2);
	m_Players.emplace_back(playerInfo);

	auto go = CreatePlayerObject(playerInfo);
	playerScene->Add(go);

	camera.SetTrackingTarget(*go.get());

	CreatePlayerActions(*go.get(), playerInfo);
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
	m_Score = 0;
}

void bomberman::PlayerManager::ResetScore()
{
	m_Score = 0;
}

bomberman::PlayerInfo bomberman::PlayerManager::CreatePlayerInfo(InputMapping mapping1, InputMapping mapping2)
{
	PlayerInfo playerInfo;

	playerInfo.playerID = static_cast<int>(m_Players.size());
	playerInfo.name = "Player " + std::to_string(playerInfo.playerID + 1);
	playerInfo.inputMappings[0] = mapping1;
	playerInfo.inputMappings[1] = mapping2;

	return playerInfo;
}

std::shared_ptr<dae::GameObject> bomberman::PlayerManager::CreatePlayerObject(const PlayerInfo& playerInfo)
{
	auto& grid = bomberman::Grid::GetInstance();
	auto& camera = dae::Camera::GetInstance();

	float tileScale{ camera.GetWindowScale() };

	bomberman::GridCell playerStartCell{ 1, 1 };
	auto go = std::make_shared<dae::GameObject>(playerInfo.name, grid.GridCoordToWorldPos(playerStartCell));
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

	return go;
}

void bomberman::PlayerManager::CreatePlayerActions(dae::GameObject& playerObject, PlayerInfo& playerInfo)
{
	auto& inputManager = dae::InputManager::GetInstance();
	float const playerSpeed{ 150.0f };

	auto upCommand = std::make_shared<bomberman::MoveCommand>(playerObject, glm::vec3(0.0f, -1.0f, 0.0f) * playerSpeed);
	auto downCommand = std::make_shared<bomberman::MoveCommand>(playerObject, glm::vec3(0.0f, 1.0f, 0.0f) * playerSpeed);
	auto leftCommand = std::make_shared<bomberman::MoveCommand>(playerObject, glm::vec3(-1.0f, 0.0f, 0.0f) * playerSpeed);
	auto rightCommand = std::make_shared<bomberman::MoveCommand>(playerObject, glm::vec3(1.0f, 0.0f, 0.0f) * playerSpeed);
	auto attackCommand = std::make_shared<bomberman::AttackCommand>(playerObject, playerInfo.playerID);

	playerInfo.inputIDs = {	inputManager.AddInputDevice(playerInfo.inputMappings[0].deviceType),
							inputManager.AddInputDevice(playerInfo.inputMappings[1].deviceType) };

	for (int i = 0; i < playerInfo.inputIDs.size(); i++)
	{
		switch (playerInfo.inputMappings[i].deviceType)
		{
		case dae::Action::DeviceType::Keyboard:
			inputManager.AddAction(dae::KeyboardKeys(playerInfo.inputMappings[i].upButton), dae::InputType::Held, upCommand,					playerInfo.inputIDs[i]);
			inputManager.AddAction(dae::KeyboardKeys(playerInfo.inputMappings[i].downButton), dae::InputType::Held, downCommand,				playerInfo.inputIDs[i]);
			inputManager.AddAction(dae::KeyboardKeys(playerInfo.inputMappings[i].leftButton), dae::InputType::Held, leftCommand,				playerInfo.inputIDs[i]);
			inputManager.AddAction(dae::KeyboardKeys(playerInfo.inputMappings[i].rightButton), dae::InputType::Held, rightCommand,				playerInfo.inputIDs[i]);
			inputManager.AddAction(dae::KeyboardKeys(playerInfo.inputMappings[i].bombButton), dae::InputType::PressedThisFrame, attackCommand,	playerInfo.inputIDs[i]);

			inputManager.AddAction(dae::KeyboardKeys::F1, dae::InputType::PressedThisFrame, std::make_shared<bomberman::SkipLevelCommand>(), playerInfo.inputIDs[i]);
			inputManager.AddAction(dae::KeyboardKeys::R, dae::InputType::PressedThisFrame, std::make_shared<bomberman::ResetLevelCommand>(), playerInfo.inputIDs[i]);
			break;
		case dae::Action::DeviceType::Gamepad:
			inputManager.AddAction(dae::GamepadButtons(playerInfo.inputMappings[i].upButton), dae::InputType::Held, upCommand,						playerInfo.inputIDs[i]);
			inputManager.AddAction(dae::GamepadButtons(playerInfo.inputMappings[i].downButton), dae::InputType::Held, downCommand,					playerInfo.inputIDs[i]);
			inputManager.AddAction(dae::GamepadButtons(playerInfo.inputMappings[i].leftButton), dae::InputType::Held, leftCommand,					playerInfo.inputIDs[i]);
			inputManager.AddAction(dae::GamepadButtons(playerInfo.inputMappings[i].rightButton), dae::InputType::Held, rightCommand,				playerInfo.inputIDs[i]);
			inputManager.AddAction(dae::GamepadButtons(playerInfo.inputMappings[i].bombButton), dae::InputType::PressedThisFrame, attackCommand,	playerInfo.inputIDs[i]);
			break;
		case dae::Action::DeviceType::UnUsed:
			continue;
		}
	}

}

void bomberman::PlayerManager::AddScore(int score)
{
	m_Score += score;

	bomberman::HUDManager::GetInstance().SetScoreText(m_Score);
}