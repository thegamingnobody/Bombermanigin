#pragma once

#include "Camera.h"

namespace bomberman
{
	#define TILE_SIZE 48.0f
	#define TILES_AMOUNT_VERTICAL 13
	#define TILES_AMOUNT_HORIZONTAL 31
	
	struct GridCell
	{
		int x{ 0 };
		int y{ 0 };

		GridCell(int x, int y)
			: x(x)
			, y(y)
		{
		}
	};

	class Grid
	{
	public:
		static glm::vec3 GridCoordToWorldPos(int x, int y)
		{
			return glm::vec3(x * TILE_SIZE, y * TILE_SIZE, 0.0f);
		}
		static glm::vec3 GridCoordToWorldPos(GridCell cell)
		{
			return GridCoordToWorldPos(cell.x, cell.y);
		}
		static glm::vec3 WorldPosToGridCoord(glm::vec3 worldPos)
		{
			return glm::vec3(worldPos.x / TILE_SIZE, worldPos.y / TILE_SIZE, 0.0f);
		}
		static GridCell WorldPosToGridCoord(float x, float y)
		{
			return GridCell(static_cast<int>(x), static_cast<int>(y));
		}
	};
}

