#include "LoadLevelState.h"
#include "SceneNames.h"
#include <SceneManager.h>
#include <TextureComponent.h>
#include "Camera.h"
#include "Grid.h"
#include "BoxCollider.h"
#include "OctagonCollider.h"
#include <Action.h>
#include "PlayerManager.h"
#include "HUDManager.h"
#include <ResourceManager.h>
#include "HUDUpdater.h"
#include "GameManager.h"
#include "MainGameState.h"
#include <InputManager.h>
#include "ExtraBombPickup.h"
#include "PickupComponent.h"
#include "EnemyManager.h"
#include "StateMachineComponent.h"
#include "PlayerIdleState.h"

bomberman::LoadLevelState::LoadLevelState(dae::GameObject& ownerObject, GameMode chosenGameMode)
	: StateMachineBase(ownerObject)
	, m_ChosenGameMode(chosenGameMode)
{
}

void bomberman::LoadLevelState::OnEnter()
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& gameManager = bomberman::GameManager::GetInstance();

	gameManager.ResetLevelCount();

	auto playerScene = sceneManager.GetScene(SCENE_PLAYERS);

	// Check if the game scene already exist 
	if (gameManager.GetGameMode() == GameMode::Selecting)
	{
		gameManager.SetGameMode(m_ChosenGameMode);
		CreateScenes();
	}

	gameManager.LoadNextLevel();
}

void bomberman::LoadLevelState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::LoadLevelState::Update(float /*deltaTime*/)
{
	return std::make_unique<bomberman::MainGameState>(*m_Owner);
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::LoadLevelState::Notify(const dae::Event& /*event*/)
{
	return nullptr;
}

void bomberman::LoadLevelState::CreateScenes()
{
	auto& sceneManager = dae::SceneManager::GetInstance();

	auto& mapScene = sceneManager.CreateScene(SCENE_MAP);
	LoadMap(mapScene);
	sceneManager.CreateScene(SCENE_OBJECTS);
	sceneManager.CreateScene(SCENE_PLAYERS);
	auto& hudScene = sceneManager.CreateScene(SCENE_HUD);
	LoadHud(hudScene);
	LoadPlayer();
}

void bomberman::LoadLevelState::LoadMap(dae::Scene& scene)
{
	auto& camera = dae::Camera::GetInstance();
	auto& grid = bomberman::Grid::GetInstance();

	auto go = std::make_shared<dae::GameObject>("Background Texture");
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Field.png");
		glm::ivec2 textureDimentions = textureComponent.GetSize();
		camera.SetCameraLimits(dae::CameraLimits(0.0f, static_cast<float>(textureDimentions.x), 0.0f, static_cast<float>(textureDimentions.y)));
	}
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("LeftWall", grid.GridCoordToWorldPos(0, 0));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, TILE_SIZE, TILE_SIZE * TILES_AMOUNT_VERTICAL));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("RightWall", grid.GridCoordToWorldPos(TILES_AMOUNT_HORIZONTAL - 1, 0));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, TILE_SIZE, TILE_SIZE * TILES_AMOUNT_VERTICAL));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("TopWall", grid.GridCoordToWorldPos(0, 0));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, TILE_SIZE * (TILES_AMOUNT_HORIZONTAL - 1), TILE_SIZE));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("BottomWall", grid.GridCoordToWorldPos(0, TILES_AMOUNT_VERTICAL - 1));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, TILE_SIZE * (TILES_AMOUNT_HORIZONTAL - 1), TILE_SIZE));
	scene.Add(go);

	bomberman::GridCell startCell{ 2, 2 };
	for (int col = 0; col < (TILES_AMOUNT_HORIZONTAL - 4) / 2 + 1; col++)
	{
		for (int row = 0; row < (TILES_AMOUNT_VERTICAL - 4) / 2 + 1; row++)
		{
			go = std::make_shared<dae::GameObject>("StaticWall_" + std::to_string(col) + "_" + std::to_string(row), grid.GridCoordToWorldPos(startCell.column + col * 2, startCell.row + row * 2));
			go->AddComponent<bomberman::OctagonCollider>(*go.get(), bomberman::CollisionType::Wall);
			scene.Add(go);
		}
	}

}

void bomberman::LoadLevelState::LoadPlayer()
{
	auto& playerManager = bomberman::PlayerManager::GetInstance();
	auto& gameManager = bomberman::GameManager::GetInstance();

	InputMapping keyboardMapping = InputMapping(dae::KeyboardKeys::W, dae::KeyboardKeys::S, dae::KeyboardKeys::A, dae::KeyboardKeys::D, dae::KeyboardKeys::C);
	InputMapping gamepadMapping = InputMapping(dae::GamepadButtons::DpadUp, dae::GamepadButtons::DpadDown, dae::GamepadButtons::DpadLeft, dae::GamepadButtons::DpadRight, dae::GamepadButtons::FaceButtonDown);

	switch (gameManager.GetGameMode())
	{
	case GameMode::Singleplayer:
		playerManager.CreatePlayer(keyboardMapping, gamepadMapping);
		break;
	case GameMode::Coop:
		// If gamepad id 1 is connected, that means there are 2 gamepads connected
		if (dae::InputManager::GetInstance().IsDeviceConnected(1))
		{
			// 2 gamepads
			// => player 1 is gamepad and keyboard, player 2 is gamepad
			playerManager.CreatePlayer(keyboardMapping, gamepadMapping);
			playerManager.CreatePlayer(gamepadMapping);
			break;
		}

		// 1 gamepad
		// => player 1 is keyboard and player 2 is gamepad
		playerManager.CreatePlayer(keyboardMapping);
		playerManager.CreatePlayer(gamepadMapping);
		break;
	case GameMode::Versus:
	{
		// If gamepad id 1 is connected, that means there are 2 gamepads connected
		if (dae::InputManager::GetInstance().IsDeviceConnected(1))
		{
			// 2 gamepads
			// => player 1 is gamepad and keyboard, player 2 is gamepad
			playerManager.CreatePlayer(keyboardMapping, gamepadMapping);
			break;
		}
		else
		{
			// 1 gamepad
			// => player 1 is keyboard and player 2 is gamepad
			playerManager.CreatePlayer(keyboardMapping);
		}

		auto& enemyManager = bomberman::EnemyManager::GetInstance();
		auto& grid = bomberman::Grid::GetInstance();

		auto go = enemyManager.CreateEnemy(bomberman::EnemyType::Balloom, grid.GridCoordToWorldPos(1, 11), false);
		playerManager.PossesObject(go, gamepadMapping);

		auto stateMachineComponent = go->GetComponent<bomberman::StateMachineComponent>();
		auto idleState = std::make_unique<bomberman::PlayerIdleState>(*go.get(), 1);
		stateMachineComponent.value()->ChangeState(std::move(idleState));

		auto scene = dae::SceneManager::GetInstance().GetScene(SCENE_PLAYERS);
		scene->Add(go);

	}
		break;
	}

}

void bomberman::LoadLevelState::LoadHud(dae::Scene& scene)
{
	auto& resourceManager = dae::ResourceManager::GetInstance();
	auto& hudManager = bomberman::HUDManager::GetInstance();
	auto windowSize = dae::Camera::GetInstance().GetWindowSize();
	uint8_t fontSize = 32;
	auto font = resourceManager.LoadFont("PixelFont.ttf", fontSize);

	// the only purpose of this gameobject is to update the HUD manager singleton and display the text
	auto go = std::make_shared<dae::GameObject>("HUD_Updater", glm::vec3(windowSize.x - (4 * TILE_SIZE), -64.0f, 0.0f));
	auto& textComponent = go->AddComponent<dae::TextComponent>(*go.get(), "Left", font);
	go->AddComponent<bomberman::HUDUpdater>(*go.get());
	scene.Add(go);

	hudManager.Init(textComponent, glm::vec3(windowSize.x - (4 * TILE_SIZE) - (7 * TILE_SIZE), -64.0f, 0.0f));
}
