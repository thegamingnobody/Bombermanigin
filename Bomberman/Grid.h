#pragma once

#include "Camera.h"
#include "Singleton.h"
#include <vector>
#include <memory>
#include <GameObject.h>
#include <Scene.h>
#include "LevelData.h"

namespace bomberman
{
	#define TILE_SIZE 48.0f
	#define TILES_AMOUNT_VERTICAL 13
	#define TILES_AMOUNT_HORIZONTAL 31

	class Grid : public dae::Singleton<Grid>
	{
	public:
		void Init();

		LevelData LoadMap(int const levelID);
		void LoadMap(LevelData const& levelData);

		void CreateGameObjects();

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

		bool IsCellTypeWalkable(CellTypes cellType) const;
		bool IsCellValid(int cellID) const;

	private:
		void LoadStaticMap();

		void CreateBrick(int gridID);
		void CreateEnemy(int gridID);
		void CreateDoor(int gridID);
		void CreatePickUp(int gridID);

		int m_BrickCount{ 0 };
		std::vector<GridCell> m_Grid{};

		bool m_DoorFound{ false };
		bool m_PickupSpawned{ false };
	};
}

