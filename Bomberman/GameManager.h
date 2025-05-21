#pragma once
#include <Singleton.h>
#include <vector>
#include "Grid.h"


namespace bomberman
{
	struct LevelData
	{
		int levelNumber{};
		int enemyCount{};

		std::vector<bomberman::GridCell> brickWalls;
		std::vector<bomberman::GridCell> enemySpawns;
		std::vector<bomberman::GridCell> playerSpawns;

		LevelData(int levelNumber, int enemyCount, std::vector<bomberman::GridCell> brickWalls, std::vector<bomberman::GridCell> enemySpawns, std::vector<bomberman::GridCell> playerSpawns)
			: levelNumber(levelNumber)
			, enemyCount(enemyCount)
			, brickWalls(brickWalls)
			, enemySpawns(enemySpawns)
			, playerSpawns(playerSpawns)
		{
		}

		LevelData() = default;
	};

	class GameManager : public dae::Singleton<GameManager>
	{
	public:
		void Init();

		LevelData GetLevelData() const { return m_CurrentLevelData; }

		void LoadNextLevel();

		void ResetLevel();

	private:
		void LoadLevel(int level); 

		int const m_MaxLevelAmount{ 3 };

		LevelData m_CurrentLevelData{};

	};

}