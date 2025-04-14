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
//#include "FPSComponent.h"
//#include "HealthComponent.h"
//#include "ScoreComponent.h"
//#include "MoveCommand.h"
//#include "AttackCommand.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& inputManager = dae::InputManager::GetInstance();
	auto& eventManager = dae::EventManager::GetInstance();
	auto& resourceManager = dae::ResourceManager::GetInstance();
	auto& camera = dae::Camera::GetInstance();

	float const tileScale{ camera.GetWindowScale() };
	float const tileSize{ 16.0f };
	float const tileSizeScaled{ tileSize * tileScale };
	int const tilesAmountVertical{ 13 };
	int const tilesAmountHorizontal{ 31 };

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

	int const player1InputID = inputManager.AddInputDevice(dae::Action::DeviceType::Gamepad);
	int const player2InputID = inputManager.AddInputDevice(dae::Action::DeviceType::Keyboard);

	go = std::make_shared<dae::GameObject>("Player 1", glm::vec3(100.0f, 100.0f, 0.0f), player1InputID);
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Bomberman_S_1.png");
		auto& healthComponent = go->AddComponent<bomberman::HealthComponent>(*go.get(), 3);
		eventManager.AddObserver(healthComponent, dae::EventType::BOMB_EXPLODED);
		auto& scoreComponent = go->AddComponent<bomberman::ScoreComponent>(*go.get());
		eventManager.AddObserver(scoreComponent, dae::EventType::OBJECT_DAMAGED);
	}

	scene.Add(go);

	float const player1Movespeed{ 100 };
	float const player2Movespeed{ player1Movespeed * 2 };

	inputManager.AddAction(dae::GamepadButtons::DpadUp, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, -1.0f, 0.0f) * player1Movespeed), player1InputID);
	inputManager.AddAction(dae::GamepadButtons::DpadDown, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, 1.0f, 0.0f) * player1Movespeed), player1InputID);
	inputManager.AddAction(dae::GamepadButtons::DpadLeft, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(-1.0f, 0.0f, 0.0f) * player1Movespeed), player1InputID);
	inputManager.AddAction(dae::GamepadButtons::DpadRight, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(1.0f, 0.0f, 0.0f) * player1Movespeed), player1InputID);
	inputManager.AddAction(dae::GamepadButtons::FaceButtonLeft, dae::InputType::PressedThisFrame, std::make_shared<bomberman::AttackCommand>(*go.get()), player1InputID);

	go = std::make_shared<dae::GameObject>("Player2", glm::vec3(48.0f, 48.0f, 0.0f), player2InputID);
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
	camera.SetTrackingTarget(*go.get());

	inputManager.AddAction(dae::KeyboardKeys::W, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, -1.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::KeyboardKeys::S, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, 1.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::KeyboardKeys::A, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(-1.0f, 0.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::KeyboardKeys::D, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(1.0f, 0.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::KeyboardKeys::C, dae::InputType::PressedThisFrame, std::make_shared<bomberman::AttackCommand>(*go.get()), player2InputID);

	////*-----------------------------------------*
	////|				   Map					  |
	////*-----------------------------------------*
	go = std::make_shared<dae::GameObject>("LeftWall", glm::vec3(0.0f, 0.0f, 0.0f));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, static_cast<float>(tileSizeScaled), static_cast<float>(tileSizeScaled * tilesAmountVertical)));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("RightWall", glm::vec3((tileSizeScaled * (tilesAmountHorizontal - 1)), 0.0f, 0.0f));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, static_cast<float>(tileSizeScaled), static_cast<float>(tileSizeScaled * tilesAmountVertical)));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("TopWall", glm::vec3(0.0f, 0.0f, 0.0f));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, static_cast<float>(tileSizeScaled * (tilesAmountHorizontal - 1)), static_cast<float>(tileSizeScaled)));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("BottomWall", glm::vec3(0.0f, static_cast<float>(tileSizeScaled * (tilesAmountVertical - 1)), 0.0f));
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Wall, bomberman::Box(0.0f, 0.0f, static_cast<float>(tileSizeScaled * (tilesAmountHorizontal - 1)), static_cast<float>(tileSizeScaled)));
	scene.Add(go);


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