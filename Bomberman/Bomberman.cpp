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

	//*-----------------------------------------*
	//|				Background stuff			|
	//*-----------------------------------------*
	auto go = std::make_shared<dae::GameObject>("Background Texture");
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("background.tga");
	}
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("Logo Texture");
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("logo.tga");
	}
	go->GetTransform()->SetLocalPosition(216.0f, 180.0f);
	scene.Add(go);

	auto font = resourceManager.LoadFont("Lingua.otf", 36);
	auto smallerFont = resourceManager.LoadFont("Lingua.otf", 18);

	go = std::make_shared<dae::GameObject>("Title 'Programming 4 Assignment' ");
	go->AddComponent<dae::TextComponent>(*go.get(), "Programming 4 Assignment", font);
	go->GetTransform()->SetLocalPosition(80.0f, 20.0f);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("FPS Counter");
	go->AddComponent<dae::TextComponent>(*go.get(), " ", font);
	go->AddComponent<bomberman::FPSComponent>(*go.get());
	go->GetTransform()->SetLocalPosition(20.0f, 430.0f);
	scene.Add(go);

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

	auto goDisplay = std::make_shared<dae::GameObject>("Player 1 Health Display", glm::vec3(10.0f, 150.0f, 0.0f));
	{
		auto& textComponent = goDisplay->AddComponent<dae::TextComponent>(*goDisplay.get(), "# Lives: 3", smallerFont, go.get());
		eventManager.AddObserver(textComponent, dae::EventType::OBJECT_DAMAGED);
	}
	scene.Add(goDisplay);

	goDisplay = std::make_shared<dae::GameObject>("Player 1 Score Display", glm::vec3(10.0f, 175.0f, 0.0f));
	{
		auto& textComponent = goDisplay->AddComponent<dae::TextComponent>(*goDisplay.get(), "Score: 0", smallerFont, go.get());
		eventManager.AddObserver(textComponent, dae::EventType::SCORE_ADDED);
	}
	scene.Add(goDisplay);

	go = std::make_shared<dae::GameObject>("Player2", glm::vec3(50.0f, 50.0f, 0.0f), player2InputID);
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("Balloom_E_1.png");
		auto& healthComponent = go->AddComponent<bomberman::HealthComponent>(*go.get(), 3);
		eventManager.AddObserver(healthComponent, dae::EventType::BOMB_EXPLODED);
		auto& scoreComponent = go->AddComponent<bomberman::ScoreComponent>(*go.get());
		eventManager.AddObserver(scoreComponent, dae::EventType::OBJECT_DAMAGED);
	}
	scene.Add(go);

	inputManager.AddAction(dae::KeyboardKeys::W, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, -1.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::KeyboardKeys::S, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(0.0f, 1.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::KeyboardKeys::A, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(-1.0f, 0.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::KeyboardKeys::D, dae::InputType::Held, std::make_shared<bomberman::MoveCommand>(*go.get(), glm::vec3(1.0f, 0.0f, 0.0f) * player2Movespeed), player2InputID);
	inputManager.AddAction(dae::KeyboardKeys::C, dae::InputType::PressedThisFrame, std::make_shared<bomberman::AttackCommand>(*go.get()), player2InputID);

	goDisplay = std::make_shared<dae::GameObject>("Player 2 Health Display", glm::vec3(10.0f, 200.0f, 0.0f));
	{
		auto& textComponent = goDisplay->AddComponent<dae::TextComponent>(*goDisplay.get(), "# Lives: 3", smallerFont, go.get());
		eventManager.AddObserver(textComponent, dae::EventType::OBJECT_DAMAGED);
	}
	scene.Add(goDisplay);

	goDisplay = std::make_shared<dae::GameObject>("Player 2 Score Display", glm::vec3(10.0f, 225.0f, 0.0f));
	{
		auto& textComponent = goDisplay->AddComponent<dae::TextComponent>(*goDisplay.get(), "Score: 0", smallerFont, go.get());
		eventManager.AddObserver(textComponent, dae::EventType::SCORE_ADDED);
	}
	scene.Add(goDisplay);

	goDisplay = std::make_shared<dae::GameObject>("Player 1 Controls Display", glm::vec3(10.0f, 60.0f, 0.0f));
	{
		auto& textComponent = goDisplay->AddComponent<dae::TextComponent>(*goDisplay.get(), "D-pad to move bomberman, X(Xbox) to attack \"place/explode\"", smallerFont);
		eventManager.AddObserver(textComponent, dae::EventType::SCORE_ADDED);
	}
	scene.Add(goDisplay);

	goDisplay = std::make_shared<dae::GameObject>("Player 2 Controls Display", glm::vec3(10.0f, 80.0f, 0.0f));
	{
		auto& textComponent = goDisplay->AddComponent<dae::TextComponent>(*goDisplay.get(), "Arrow keys to move balloom, C to attack \"place/explode\"", smallerFont);
		eventManager.AddObserver(textComponent, dae::EventType::SCORE_ADDED);
	}
	scene.Add(goDisplay);


	////*-----------------------------------------*
	////|				     ImGui				  |
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