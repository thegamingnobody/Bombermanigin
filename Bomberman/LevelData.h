#pragma once
#include <vector>

namespace bomberman
{
	enum class CellTypes
	{
		Empty,
		Wall,
		Brick,
		PlayerSpawn,
		EnemySpawn,
		Door
	};

	struct GridCell
	{
		int column{ 0 };
		int row{ 0 };
		CellTypes cellType{ CellTypes::Empty };

		GridCell(int columnTile, int rowTile, CellTypes cellType = CellTypes::Empty)
			: column(columnTile)
			, row(rowTile)
			, cellType(cellType)
		{
		}

		GridCell()
			: column(0)
			, row(0)
			, cellType(CellTypes::Empty)
		{
		}
	};

	//Declared in its own header file to avoid circular dependencies
	struct LevelData
	{
		int levelNumber{};
		int enemyCount{};

		std::vector<bomberman::GridCell> brickWalls;
		std::vector<bomberman::GridCell> enemySpawns;
		std::vector<bomberman::GridCell> playerSpawns;
		std::vector<int> enemyTypes;

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
} 
