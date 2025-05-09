#pragma once

#include "Camera.h"
#include "Singleton.h"
#include <vector>
#include <memory>
#include <GameObject.h>
#include <Scene.h>

namespace bomberman
{
	#define TILE_SIZE 48.0f
	#define TILES_AMOUNT_VERTICAL 13
	#define TILES_AMOUNT_HORIZONTAL 31
	
	enum class CellTypes
	{
		Empty,
		Wall,
		Brick,
		PlayerSpawn,
		EnemySpawn
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

	class Grid : public dae::Singleton<Grid>
	{
	public:
		void Init();

		void LoadMap(int const levelID);
		void CreateGameObjects(dae::Scene& scene);

		glm::vec3 GridCoordToWorldPos(int column, int row)
		{
			return glm::vec3(column * TILE_SIZE, row * TILE_SIZE, 0.0f);
		}
		glm::vec3 GridCoordToWorldPos(GridCell cell)
		{
			return GridCoordToWorldPos(cell.column, cell.row);
		}
		
		glm::vec3 WorldPosToGridCoord(float x, float y)
		{
			return glm::vec3(x / TILE_SIZE, y / TILE_SIZE, 0.0f);
		}
		glm::vec3 WorldPosToGridCoord(glm::vec3 worldPos)
		{
			return WorldPosToGridCoord(worldPos.x, worldPos.y);
		}
		
		void BrickDestroyed(int cellID);

		GridCell GetCell(int cellID) const;
		GridCell GetCell(int column, int row) const;

		int GetCellID(int column, int row) const;
	private:
		void CreateBrick(dae::Scene& scene, int gridID);

		int m_LevelID{ 0 };
		int m_BrickCount{ 0 };
		//int m_MaxBombs{ 0 };
		std::vector<GridCell> m_Grid{};
	};
}

