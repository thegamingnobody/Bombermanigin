#include "GameManager.h"
#include <SceneManager.h>
#include <Scene.h>
#include "Grid.h"
#include "SceneNames.h"
#include <TextureComponent.h>
#include "BoxCollider.h"
#include "OctagonCollider.h"

void bomberman::GameManager::ResetLevelCount()
{
	m_CurrentLevelData = bomberman::LevelData{};
	m_GameMode = GameMode::Selecting;
}

void bomberman::GameManager::LoadLevel(int level)
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& grid = bomberman::Grid::GetInstance();

	auto objectsScene =	sceneManager.GetScene(SCENE_OBJECTS);

	objectsScene->RemoveAll();

	m_CurrentLevelData = grid.LoadMap(level - 1);
	grid.CreateGameObjects();

	ResetPlayerPositions();
}

void bomberman::GameManager::ResetPlayerPositions()
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& grid = bomberman::Grid::GetInstance();

	auto playerScene = sceneManager.GetScene(SCENE_PLAYERS);
	auto player = playerScene->GetObject("Player 1");

	if (player == nullptr) return;

	player->GetTransform()->SetLocalPosition(grid.GridCoordToWorldPos(m_CurrentLevelData.playerSpawns[0]));

	player = playerScene->GetObject("Player 2");

	if (player == nullptr) return;

	player->GetTransform()->SetLocalPosition(grid.GridCoordToWorldPos(m_CurrentLevelData.playerSpawns[1]));
}

void bomberman::GameManager::LoadNextLevel()
{
	if (m_CurrentLevelData.levelNumber < m_MaxLevelAmount)
	{
		LoadLevel(m_CurrentLevelData.levelNumber + 1);
		return;
	}

	LoadLevel(m_MaxLevelAmount);
}

void bomberman::GameManager::ResetLevel()
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& grid = bomberman::Grid::GetInstance();

	auto objectsScene =	sceneManager.GetScene(SCENE_OBJECTS);
	
	// Object Scene
	objectsScene->RemoveAll();
	grid.LoadMap(m_CurrentLevelData);
	grid.CreateGameObjects();

	// Player Scene
	ResetPlayerPositions();
}
