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
#include "OctagonCollider.h"
#include "EventTypes.h"
#include <DAE_SDL_Soundsystem.h>
#include <ServiceLocator.h>
#include "SoundIds.h"
#include <ImGuiComponent.h>
#include "EnemyManager.h"
#include "EnemyMovementComponent.h"

//Todo: add pickups

//Enemy behavior:
// 1. Balloom 
//		Speed: 2, slow	
//		Score: 100
//		1) Move randomly
//		2) Change direction at intersection( or bump into wall)
// 2. Oneal 
//		Speed: 3, normal
// 		Score: 200
// 		1) Move randomly, but greater chance to move towards player at intersection
// 3. Doll
//		Speed: 3, normal
// 		Score: 400
// 4. Minvo
//		Speed: 4, fast
//		Score: 800

void LoadSounds();
void LoadMap(dae::Scene& scene);
void LoadPlayer(dae::Scene& scene);
void LoadEnemies(dae::Scene& scene);

void load()
{
	bomberman::EnemyManager::GetInstance().Init();
	bomberman::Grid::GetInstance().Init();

	LoadSounds();

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Game");

	LoadMap(scene);

	//auto font = resourceManager.LoadFont("Lingua.otf", 36);
	//auto smallerFont = resourceManager.LoadFont("Lingua.otf", 18);

	LoadPlayer(scene);

	LoadEnemies(scene);

	////*-----------------------------------------*
	////|				  ImGui 				  |
	////*-----------------------------------------*
	//auto go = std::make_shared<dae::GameObject>("ImGui");
	//go->AddComponent<dae::ImGuiComponent>(*go.get());
	//scene.Add(go);
}

void LoadSounds()
{
	dae::ServiceLocator::RegisterSoundSystem(new dae::DAE_SDL_Soundsystem());

	std::vector<std::string> SoundPaths =
	{
		"WalkHorizontal.wav",
		"WalkVertical.wav"
	};

	dae::ServiceLocator::GetSoundSystem().AddSound(static_cast<int>(bomberman::SoundId::WalkHorizontal), SoundPaths[static_cast<int>(bomberman::SoundId::WalkHorizontal)]);
	dae::ServiceLocator::GetSoundSystem().AddSound(static_cast<int>(bomberman::SoundId::WalkVertical), SoundPaths[static_cast<int>(bomberman::SoundId::WalkVertical)]);
}

void LoadMap(dae::Scene& scene)
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

	grid.LoadMap(0);
	grid.CreateGameObjects(scene);


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

void LoadPlayer(dae::Scene& scene)
{
	auto& inputManager = dae::InputManager::GetInstance();
	auto& eventManager = dae::EventManager::GetInstance();
	auto& camera = dae::Camera::GetInstance();
	auto& grid = bomberman::Grid::GetInstance();

	float tileScale{ camera.GetWindowScale() };

	int const player1InputID = inputManager.AddInputDevice(dae::Action::DeviceType::Keyboard);
	//int const player2InputID = inputManager.AddInputDevice(dae::Action::DeviceType::Gamepad);

	bomberman::GridCell playerStartCell{ 1, 1 };
	auto go = std::make_shared<dae::GameObject>("Player 1", grid.GridCoordToWorldPos(playerStartCell), player1InputID);
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Bomberman_S_1.png");
		auto textureSize = textureComponent.GetSize();
		auto& healthComponent = go->AddComponent<bomberman::HealthComponent>(*go.get(), 3);
		eventManager.AddObserver(healthComponent, static_cast<int>(bomberman::EventType::BOMB_EXPLODED));
		auto& scoreComponent = go->AddComponent<bomberman::ScoreComponent>(*go.get());
		eventManager.AddObserver(scoreComponent, static_cast<int>(bomberman::EventType::OBJECT_DAMAGED));
		//float const hitboxOffset{ 2.0f };
		go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Player, bomberman::Box(4 * tileScale, 1 * tileScale, 8 * tileScale, 14 * tileScale));
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

	//bomberman::GridCell enemyStartCell{ 4, 1 };
	//go = std::make_shared<dae::GameObject>("Player2", bomberman::Grid::GridCoordToWorldPos(enemyStartCell), player2InputID);
	//{
	//	auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
	//	textureComponent.AddTexture("Balloom_E_1.png");
	//	auto textureSize = textureComponent.GetSize();
	//	auto& healthComponent = go->AddComponent<bomberman::HealthComponent>(*go.get(), 3);
	//	eventManager.AddObserver(healthComponent, static_cast<int>(bomberman::EventType::BOMB_EXPLODED));
	//	auto& scoreComponent = go->AddComponent<bomberman::ScoreComponent>(*go.get());
	//	eventManager.AddObserver(scoreComponent, static_cast<int>(bomberman::EventType::OBJECT_DAMAGED));
	//	float const hitboxOffset{ 2.0f };
	//	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Entity, bomberman::Box(hitboxOffset, hitboxOffset, static_cast<float>(textureSize.x) - hitboxOffset * 2, static_cast<float>(textureSize.y) - hitboxOffset * 2));
	//}
	//scene.Add(go);

	//inputManager.AddAction(dae::GamepadButtons::DpadUp, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, -1.0f, 0.0f) * player2Movespeed), player2InputID);
	//inputManager.AddAction(dae::GamepadButtons::DpadDown, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, 1.0f, 0.0f) * player2Movespeed), player2InputID);
	//inputManager.AddAction(dae::GamepadButtons::DpadLeft, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(-1.0f, 0.0f, 0.0f) * player2Movespeed), player2InputID);
	//inputManager.AddAction(dae::GamepadButtons::DpadRight, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(1.0f, 0.0f, 0.0f) * player2Movespeed), player2InputID);
	//inputManager.AddAction(dae::GamepadButtons::FaceButtonLeft, dae::InputType::PressedThisFrame, std::make_shared<bomberman::AttackCommand>(*go.get()), player2InputID);

}

void LoadEnemies(dae::Scene& scene)
{
	auto& enemyManager = bomberman::EnemyManager::GetInstance();
	auto& grid = bomberman::Grid::GetInstance();

	auto go = enemyManager.CreateEnemy(bomberman::EnemyType::Balloom, grid.GridCoordToWorldPos(3, 3));
	scene.Add(go);
}

int main(int, char* []) 
{
	//First one is for when running in Visual Studio, the second one is for the build
	//Could maybe use a better solution but haven't come up with one yet
	std::filesystem::path dataPath = "../Data/";
	std::filesystem::path dataPath2 = "Data/";
	std::string actualPath{};

	if (std::filesystem::exists(dataPath))
	{
		actualPath = dataPath.string();
	}
	else if (std::filesystem::exists(dataPath2))
	{
		actualPath = dataPath2.string();
	}
	else
	{
		std::cerr << "Data path not found!" << std::endl;
		return -1;
	}

	dae::Minigin engine(actualPath);
	engine.Run(load);

	return 0;
}	