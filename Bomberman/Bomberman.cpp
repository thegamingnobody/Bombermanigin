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
#include "GameManager.h"
#include "SkipLevelCommand.h"
#include "ResetLevelCommand.h"
#include "CollidersManager.h"
#include "PlayerManager.h"
#include "HUDManager.h"
#include <Renderer.h>
#include "HUDUpdater.h"
#include "SceneNames.h"
#include "StateMachineComponent.h"
#include "MenuState.h"
#include "CursorMoveCommand.h"
#include "CursorSelectCommand.h"

//Todo: add pickups
//Todo: check vr nog classes final te maken als mogelijk
void LoadSounds();
void LoadMenu(dae::Scene& scene);
void CreateGameState(dae::Scene& scene);

void load()
{
	bomberman::EnemyManager::GetInstance().Init();
	bomberman::Grid::GetInstance().Init();
	bomberman::CollidersManager::GetInstance().Init();

	auto& sceneManager = dae::SceneManager::GetInstance();

	dae::Renderer::GetInstance().SetBackgroundColor(SDL_Color(161, 161, 161));

	LoadSounds();

	// 1 scene for the main menu											"Menu"
	// 1 scene for the static elements (background texture, walls, etc.)	"Map"
	// 1 scene for the player												"Player"
	// 1 scene for the enemies, bricks, pickups, etc.						"Objects"
	// 1 scene for the HUD													"Hud"
	// possibly more if needed
	
	// This way we can easily reset the enemies, pickups, etc. without removing the map or player

	auto& menuScene = sceneManager.CreateScene(SCENE_MAIN_MENU);
	auto& gameStateScene = sceneManager.CreateScene(SCENE_GAME_STATE);

	CreateGameState(gameStateScene);
	LoadMenu(menuScene);
}

void LoadSounds()
{
	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

	std::vector<std::string> SoundPaths =
	{
		"WalkHorizontal.wav",
		"WalkVertical.wav",
		"BombPlace.wav"
	};

	soundSystem.AddSound(static_cast<int>(bomberman::SoundId::WalkHorizontal), SoundPaths[static_cast<int>(bomberman::SoundId::WalkHorizontal)]);
	soundSystem.AddSound(static_cast<int>(bomberman::SoundId::WalkVertical), SoundPaths[static_cast<int>(bomberman::SoundId::WalkVertical)]);
}

void LoadMenu(dae::Scene& scene)
{
	auto& inputManager = dae::InputManager::GetInstance();

	auto go = std::make_shared<dae::GameObject>("Title_Screen_Image", glm::vec3(0.0f, -2 * TILE_SIZE, 0.0f));
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("TitleScreen.png");
	}
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("Cursor", glm::vec3(5 * TILE_SIZE, static_cast<int>(bomberman::CursorOptions::SinglePlayer) * TILE_SIZE, 1.0f));
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Cursor.png");
	}
	scene.Add(go);

	int keyboardID = inputManager.AddInputDevice(dae::Action::DeviceType::Keyboard);
	int controllerID = inputManager.AddInputDevice(dae::Action::DeviceType::Gamepad);

	inputManager.AddAction(dae::KeyboardKeys::W, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), false), keyboardID);
	inputManager.AddAction(dae::KeyboardKeys::S, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), true),  keyboardID);

	inputManager.AddAction(dae::KeyboardKeys::Up, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), false), keyboardID);
	inputManager.AddAction(dae::KeyboardKeys::Down, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), true),  keyboardID);
	
	inputManager.AddAction(dae::KeyboardKeys::Space, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorSelectCommand>(*go.get()),  keyboardID);
	
	inputManager.AddAction(dae::GamepadButtons::DpadUp, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), false), controllerID);
	inputManager.AddAction(dae::GamepadButtons::DpadDown, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorMoveCommand>(*go.get(), true), controllerID);
	
	inputManager.AddAction(dae::GamepadButtons::FaceButtonDown, dae::InputType::PressedThisFrame, std::make_shared<bomberman::CursorSelectCommand>(*go.get()), controllerID);


}

void CreateGameState(dae::Scene& scene)
{
	auto go = std::make_shared<dae::GameObject>("GameState");
	auto& stateMachineComponent = go->AddComponent<bomberman::StateMachineComponent>(*go.get());
	auto menuState = std::make_unique<bomberman::MenuState>(*go.get());
	stateMachineComponent.ChangeState(std::move(menuState));
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