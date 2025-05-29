#include "PlayerManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "Grid.h"
#include <TextureComponent.h>
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "BoxCollider.h"
#include "MoveCommand.h"
#include "AttackCommand.h"
#include "SkipLevelCommand.h"
#include "ResetLevelCommand.h"
#include "StateMachineComponent.h"
#include "PlayerIdleState.h"
#include "SceneNames.h"

void bomberman::PlayerManager::CreatePlayer(dae::Action::DeviceType deviceType)
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
	
	// Todo: add state machine to player

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

	// Todo: change to use keyboard or gamepad
	inputManager.AddAction(dae::KeyboardKeys::W, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, -1.0f, 0.0f) * playerSpeed), playerInputID);
	inputManager.AddAction(dae::KeyboardKeys::S, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, 1.0f, 0.0f)	* playerSpeed), playerInputID);
	inputManager.AddAction(dae::KeyboardKeys::A, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(-1.0f, 0.0f, 0.0f) * playerSpeed), playerInputID);
	inputManager.AddAction(dae::KeyboardKeys::D, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(1.0f, 0.0f, 0.0f)	* playerSpeed), playerInputID);
	inputManager.AddAction(dae::KeyboardKeys::C, dae::InputType::PressedThisFrame, std::make_shared<bomberman::AttackCommand>(*go.get()), playerInputID);

	// Todo: should only be added once
	inputManager.AddAction(dae::KeyboardKeys::F1, dae::InputType::PressedThisFrame, std::make_shared<bomberman::SkipLevelCommand>(), playerInputID);
	inputManager.AddAction(dae::KeyboardKeys::R, dae::InputType::PressedThisFrame, std::make_shared<bomberman::ResetLevelCommand>(), playerInputID);
}
