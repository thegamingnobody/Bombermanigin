#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <Minigin.h>
#include <memory>
#include <SceneManager.h>
#include <InputManager.h>
#include <EventManager.h>
#include <GameObject.h>
#include <TextureComponent.h>
#include <Scene.h>
#include <ResourceManager.h>
#include <TextComponent.h>
#include "FPSComponent.h"
#include "HealthComponent.h"
#include "AttackCommand.h"
#include "MoveCommand.h"
#include "ScoreComponent.h"
#include <Camera.h>
#include "BoxCollider.h"
#include "Grid.h"



void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& inputManager = dae::InputManager::GetInstance();
	auto& eventManager = dae::EventManager::GetInstance();
	auto& resourceManager = dae::ResourceManager::GetInstance();
	auto& camera = dae::Camera::GetInstance();

	float tileScale{ camera.GetWindowScale() };
	//*-----------------------------------------*
	//|				Background stuff			|
	//*-----------------------------------------*
	auto go = std::make_shared<dae::GameObject>("Background Texture");
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Field.png");
		glm::ivec2 textureDimentions = textureComponent.GetSize();
		camera.SetCameraLimits(dae::CameraLimits(0.0f, static_cast<float>(textureDimentions.x), 0.0f, static_cast<float>(textureDimentions.y)));
	}
	scene.Add(go);

	auto font = resourceManager.LoadFont("Lingua.otf", 36);
	auto smallerFont = resourceManager.LoadFont("Lingua.otf", 18);

	//*-----------------------------------------*
	//|				     Player					|
	//*-----------------------------------------*

	int const player1InputID = inputManager.AddInputDevice(dae::Action::DeviceType::Keyboard);
	int const player2InputID = inputManager.AddInputDevice(dae::Action::DeviceType::Gamepad);

	bomberman::GridCell playerStartCell{ 1, 1 };
	go = std::make_shared<dae::GameObject>("Player 1", bomberman::Grid::GridCoordToWorldPos(playerStartCell), player1InputID);
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Bomberman_S_1.png");
		auto textureSize = textureComponent.GetSize();
		auto& healthComponent = go->AddComponent<bomberman::HealthComponent>(*go.get(), 3);
		eventManager.AddObserver(healthComponent, dae::EventType::BOMB_EXPLODED);
		auto& scoreComponent = go->AddComponent<bomberman::ScoreComponent>(*go.get());
		eventManager.AddObserver(scoreComponent, dae::EventType::OBJECT_DAMAGED);
		//float const hitboxOffset{ 2.0f };
		go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Entity, bomberman::Box(4 * tileScale, 0, 8 * tileScale, 16 * tileScale));
	}
	scene.Add(go);
	camera.SetTrackingTarget(*go.get());

	float const player2Movespeed{ 100 };
	float const player1Movespeed{ player2Movespeed * 2 };

	inputManager.AddAction(dae::KeyboardKeys::W, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, -1.0f, 0.0f) * player1Movespeed), player1InputID);
	inputManager.AddAction(dae::KeyboardKeys::S, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, 1.0f, 0.0f) * player1Movespeed), player1InputID);
	inputManager.AddAction(dae::KeyboardKeys::A, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(-1.0f, 0.0f, 0.0f) * player1Movespeed), player1InputID);
	inputManager.AddAction(dae::KeyboardKeys::D, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(1.0f, 0.0f, 0.0f) * player1Movespeed), player1InputID);
	inputManager.AddAction(dae::KeyboardKeys::C, dae::InputType::PressedThisFrame, std::make_shared<bomberman::AttackCommand>(*go.get()), player1InputID);

	bomberman::GridCell enemyStartCell{ 4, 1 };
	go = std::make_shared<dae::GameObject>("Player2", bomberman::Grid::GridCoordToWorldPos(enemyStartCell), player2InputID);
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Balloom_E_1.png");
		auto textureSize = textureComponent.GetSize();
		auto& healthComponent = go->AddComponent<bomberman::HealthComponent>(*go.get(), 3);
		eventManager.AddObserver(healthComponent, dae::EventType::BOMB_EXPLODED);
		auto& scoreComponent = go->AddComponent<bomberman::ScoreComponent>(*go.get());
		eventManager.AddObserver(scoreComponent, dae::EventType::OBJECT_DAMAGED);
		float const hitboxOffset{ 2.0f };
		go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Entity, bomberman::Box(hitboxOffset, hitboxOffset, static_cast<float>(textureSize.x) - hitboxOffset * 2, static_cast<float>(textureSize.y) - hitboxOffset * 2));
	}
	scene.Add(go);

	inputManager.AddAction(dae::GamepadButtons::DpadUp, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, -1.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::GamepadButtons::DpadDown, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, 1.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::GamepadButtons::DpadLeft, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(-1.0f, 0.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::GamepadButtons::DpadRight, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(1.0f, 0.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::GamepadButtons::FaceButtonLeft, dae::InputType::PressedThisFrame, std::make_shared<bomberman::AttackCommand>(*go.get()), player2InputID);

	////*-----------------------------------------*
	////|				   Map					  |
	////*-----------------------------------------*
	go = std::make_shared<dae::GameObject>("LeftWall", bomberman::Grid::GridCoordToWorldPos(0, 0));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, TILE_SIZE, TILE_SIZE * TILES_AMOUNT_VERTICAL));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("RightWall", bomberman::Grid::GridCoordToWorldPos(TILES_AMOUNT_HORIZONTAL - 1, 0));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, TILE_SIZE, TILE_SIZE * TILES_AMOUNT_VERTICAL));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("TopWall", bomberman::Grid::GridCoordToWorldPos(0, 0));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, TILE_SIZE * (TILES_AMOUNT_HORIZONTAL - 1), TILE_SIZE));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("BottomWall", bomberman::Grid::GridCoordToWorldPos(0, TILES_AMOUNT_VERTICAL - 1));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, TILE_SIZE * (TILES_AMOUNT_HORIZONTAL - 1), TILE_SIZE));
	scene.Add(go);

	//bomberman::GridCell startCell{ 2, 2 };
	//for (int col = 0; col < (TILES_AMOUNT_HORIZONTAL - 4)/2; col++)
	//{
	//	for (int row = 0; row < (TILES_AMOUNT_VERTICAL - 4)/2+1; row++)
	//	{
	//		go = std::make_shared<dae::GameObject>("StaticWall_" + std::to_string(col) + "_" + std::to_string(row), bomberman::Grid::GridCoordToWorldPos(startCell.x + col * 2, startCell.y + row * 2));
	//		go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, tileSizeScaled, tileSizeScaled));
	//		scene.Add(go);
	//	}
	//}

	////*-----------------------------------------*
	////|				  ImGui 				  |
	////*-----------------------------------------*
	//go = std::make_shared<dae::GameObject>("ImGui");
	//go->AddComponent<dae::ImGuiComponent>(*go.get());
	//scene.Add(go);
}

int main(int, char* []) 
{
    dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}	