#include "GameOverMenuState.h"
#include <SceneManager.h>
#include <InputManager.h>
#include "SceneNames.h"
#include <GameObject.h>
#include <Scene.h>
#include "Grid.h"
#include <TextComponent.h>
#include <ResourceManager.h>
#include "IncDecLetterCommand.h"
#include "NameEntryManager.h"
#include "SwitchLetterIndex.h"
#include <SDL_ttf.h>
#include "StateMachineComponent.h"
#include "EventTypes.h"
#include <EventManager.h>
#include "MenuState.h"
#include "NameConfirmCommand.h"
#include "PlayerManager.h"
#include <string> 
#include <fstream>
#include "NameConfirmEvent.h"

bomberman::GameOverMenuState::GameOverMenuState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::GameOverMenuState::OnEnter()
{
	auto& sceneManager = dae::SceneManager::GetInstance();

	sceneManager.RemoveScene(SCENE_MAP);
	sceneManager.RemoveScene(SCENE_OBJECTS);
	sceneManager.RemoveScene(SCENE_PLAYERS);
	sceneManager.RemoveScene(SCENE_HUD);

	CreateGameOverScreen();

	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	dae::EventManager::GetInstance().AddObserver(*stateMachineComp.value(), static_cast<int>(bomberman::EventType::NAME_CONFIRM));

}

void bomberman::GameOverMenuState::OnExit()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	dae::EventManager::GetInstance().RemoveObserver(*stateMachineComp.value());

	dae::Camera::GetInstance().EnableTracking(false);

	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.RemoveInputDevice(dae::Action::DeviceType::Keyboard);
	inputManager.RemoveInputDevice(dae::Action::DeviceType::Gamepad);

	sceneManager.RemoveScene(SCENE_GAMEOVER);

	auto& playerManager = bomberman::PlayerManager::GetInstance();
	playerManager.ClearPlayers();
	playerManager.ResetScore();
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::GameOverMenuState::Update(float /*deltaTime*/)
{
	return nullptr;
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::GameOverMenuState::Notify(const dae::Event& event)
{ 
	switch (static_cast<bomberman::EventType>(event.GetEventType()))
	{
	case bomberman::EventType::NAME_CONFIRM:
	{
		//Todo: save name & score to file
		auto& castedEvent = static_cast<const bomberman::NameConfirmEvent&>(event);
		ScoreEntry newScoreEntry{ castedEvent.GetName(),  bomberman::PlayerManager::GetInstance().GetScore() };

		SaveScores(newScoreEntry);

		return std::make_unique<bomberman::MenuState>(*m_Owner);
	}
	default:
		break;
	}

	return nullptr;
}

void bomberman::GameOverMenuState::CreateGameOverScreen()
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& inputManager = dae::InputManager::GetInstance();
	auto& resourceManager = dae::ResourceManager::GetInstance();
	auto& playerManager = bomberman::PlayerManager::GetInstance();

	auto& gameOverScene = sceneManager.CreateScene(SCENE_GAMEOVER);

#pragma region CameraReset

	auto go = std::make_shared<dae::GameObject>("TrackingTargetReset");
	gameOverScene.Add(go);
	dae::Camera::GetInstance().SetTrackingTarget(*go.get());

#pragma endregion
	
#pragma region Font
	uint8_t fontSize = 32;
	float biggerFontScale = 1.5f;
	auto font = resourceManager.LoadFont("PixelFont.ttf", fontSize);
	auto biggerFont = resourceManager.LoadFont("PixelFont.ttf", static_cast<uint8_t>(fontSize * biggerFontScale));
	int width;
	int height;
	// Get width of a single character in the smaller font to space the letters and cursor correctly
	TTF_SizeText(font->GetFont(), "A", &width, &height);
#pragma endregion

#pragma region DefaultElements
	auto middleLetterX = TILE_SIZE * 7;

	go = std::make_shared<dae::GameObject>("GameOverText", glm::vec3(middleLetterX - ((width * biggerFontScale) * 4), TILE_SIZE * 2, 0.0f));
	go->AddComponent<dae::TextComponent>(*go.get(), "Game Over!", biggerFont);
	gameOverScene.Add(go);

	int currentScore = playerManager.GetScore();
	std::string scoreText = "Score: " + std::to_string(currentScore);
	go = std::make_shared<dae::GameObject>("Score", glm::vec3(middleLetterX - width - (scoreText.length()/2), TILE_SIZE * 3, 0.0f));
	go->AddComponent<dae::TextComponent>(*go.get(), scoreText, font);
	gameOverScene.Add(go);

	LoadScores();

	for (int i = 0; i < m_Scores.size(); i++)
	{
		std::string scoreEntry = m_Scores[i].first + " " + std::to_string(m_Scores[i].second);
		go = std::make_shared<dae::GameObject>("HighScoreEntry", glm::vec3(15.0f, TILE_SIZE * (i+8), 0.0f));
		go->AddComponent<dae::TextComponent>(*go.get(), scoreEntry, font);
		gameOverScene.Add(go);
	}

	inputManager.RemoveInputDevice(dae::Action::DeviceType::Keyboard);
	inputManager.RemoveInputDevice(dae::Action::DeviceType::Gamepad);
	// Todo: add gamepad controls
	int keyboardID = inputManager.AddInputDevice(dae::Action::DeviceType::Keyboard);
	inputManager.AddAction(dae::KeyboardKeys::Enter, dae::InputType::PressedThisFrame, std::make_shared<bomberman::NameConfirmCommand>(), keyboardID);
#pragma endregion

#pragma region NameEntry

	if (currentScore <= m_Scores[4].second)
	{
		// Only Enter Name if the score is high enough
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		go = std::make_shared<dae::GameObject>("Letter"+std::to_string(i), glm::vec3(middleLetterX + (width * (i - 1)), TILE_SIZE * 6, 0.0f));
		go->AddComponent<dae::TextComponent>(*go.get(), "A", font);
		gameOverScene.Add(go);
	}

	go = std::make_shared<dae::GameObject>("Cursor", glm::vec3(middleLetterX - width, TILE_SIZE * 5, 0.0f));
	go->AddComponent<dae::TextComponent>(*go.get(), "V  ", font);
	gameOverScene.Add(go);
#pragma endregion

#pragma region NameEntryInputSetup
	inputManager.AddAction(dae::KeyboardKeys::W, dae::InputType::PressedThisFrame, std::make_shared<IncDecLetterCommand>(true), keyboardID);
	inputManager.AddAction(dae::KeyboardKeys::S, dae::InputType::PressedThisFrame, std::make_shared<IncDecLetterCommand>(false), keyboardID);

	inputManager.AddAction(dae::KeyboardKeys::A, dae::InputType::PressedThisFrame, std::make_shared<SwitchLetterIndex>(false), keyboardID);
	inputManager.AddAction(dae::KeyboardKeys::D, dae::InputType::PressedThisFrame, std::make_shared<SwitchLetterIndex>(true), keyboardID);	
#pragma endregion

	bomberman::NameEntryManager::GetInstance().Reset();
}

void bomberman::GameOverMenuState::SortAndTrimScores()
{
	std::sort(m_Scores.begin(), m_Scores.end(), [](const ScoreEntry& a, const ScoreEntry& b)
		{
			return a.second > b.second;
		});

	if (m_Scores.size() > m_MaxScores)
	{
		m_Scores.resize(m_MaxScores);
	}
	else if (m_Scores.size() < m_MaxScores)
	{
		for (int i = 0; i <= (m_MaxScores - m_Scores.size()); i++)
		{
			m_Scores.emplace_back("---", 0);
		}
	}

}

void bomberman::GameOverMenuState::LoadScores()
{
	m_Scores.clear();
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
			return;
		}
	}

	std::string line;
	std::string name;
	int score;

	while (f >> name >> score)
	{
		m_Scores.emplace_back(name, score);
	}

	SortAndTrimScores();
}

void bomberman::GameOverMenuState::SaveScores(ScoreEntry newScoreEntry)
{
	m_Scores.emplace_back(newScoreEntry);

	SortAndTrimScores();

	std::filesystem::path filePath = __FILE__;
	std::filesystem::path fileDir = filePath.parent_path();
	auto dataPath = "Scores\\Scores.txt";

	fileDir.append(dataPath);
	std::ofstream f(fileDir.c_str());

	// Fallback if primary path fails
	if (!f.is_open())
	{
		f = std::ofstream(dataPath);
		if (!f.is_open())
		{
			return;
		}
	}

	for (const auto& [name, score] : m_Scores)
	{
		f << name << ' ' << score << '\n';
	}
}
