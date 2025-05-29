#include "GameManager.h"
#include <SceneManager.h>
#include <Scene.h>
#include "Grid.h"
#include "SceneNames.h"

// Todo: Add win condition to gamemanager
// Todo: disable current scene and create a new scene for the next level
//			=> store score in gamemanager
// Todo: add a game over scene

void bomberman::GameManager::LoadLevel(int level)
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& grid = bomberman::Grid::GetInstance();

	auto objectsScene =	sceneManager.GetScene(SCENE_OBJECTS);
	auto playerScene = sceneManager.GetScene(SCENE_PLAYERS);

	if (objectsScene == nullptr)
	{
		throw std::runtime_error("Objects scene not found!");
	}
	objectsScene->RemoveAll();

	m_CurrentLevelData = grid.LoadMap(level - 1);
	grid.CreateGameObjects();

	auto player = playerScene->GetObject("Player 1");

	if (player == nullptr) return;

	player->GetTransform()->SetLocalPosition(grid.GridCoordToWorldPos(1, 1));
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
	auto playerScene = sceneManager.GetScene(SCENE_PLAYERS);
	
	// Object Scene
	objectsScene->RemoveAll();
	grid.LoadMap(m_CurrentLevelData);
	grid.CreateGameObjects();

	// Player Scene

	// Todo: fix for multiplayer
	playerScene->GetObject("Player 1")->GetTransform()->SetLocalPosition(grid.GridCoordToWorldPos(1, 1));

}
