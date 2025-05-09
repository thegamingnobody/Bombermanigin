#include "Grid.h"
#include <json.hpp>
#include <fstream>
#include <TextureComponent.h>
#include "HealthComponent.h"
#include "BoxCollider.h"


using json = nlohmann::json;

void bomberman::Grid::Init()
{
	m_Grid.resize(TILES_AMOUNT_HORIZONTAL * TILES_AMOUNT_VERTICAL);
	for (int row = 0; row < TILES_AMOUNT_VERTICAL; row++)
	{
		for (int column = 0; column < TILES_AMOUNT_HORIZONTAL; column++)
		{
			GridCell newCell = GridCell(column, row, (column == 0 or row == 0 or column + 1 == TILES_AMOUNT_HORIZONTAL or row + 1 == TILES_AMOUNT_VERTICAL) ? CellTypes::Wall : CellTypes::Empty);
			m_Grid[column + row * TILES_AMOUNT_HORIZONTAL] = newCell;
		}
	}
}

void bomberman::Grid::LoadMap(int const levelID)
{
	std::filesystem::path filePath = __FILE__;
	std::filesystem::path fileDir = filePath.parent_path();
	auto dataPath = "LevelData\\LevelData.json";

	fileDir.append(dataPath);
	std::ifstream f(fileDir.c_str());
	std::ifstream f2(dataPath);
	json data;

	try
	{
		data = json::parse(f);
	}
	catch (...)
	{
		data = json::parse(f2);
	}

	if (levelID == static_cast<int>(data.size()))
	{
		throw std::out_of_range("Level ID out of range");
	}

	m_LevelID = data[levelID]["level"];
	auto jsonBrick = data[levelID]["bricks"];
	for (int column = 1; column <= jsonBrick.size(); column++)
	{
		auto brick = jsonBrick[column - 1];
		for (int row = 0; row < brick["y"].size(); row++)
		{
			GridCell brickToAdd = GridCell(column, brick["y"][row], CellTypes::Brick);
			int cellNumber = (brickToAdd.row * TILES_AMOUNT_HORIZONTAL) + brickToAdd.column;

			if (cellNumber >= static_cast<int>(m_Grid.size()) or cellNumber < 0) { throw std::out_of_range("Cell number out of range"); }
			m_Grid[cellNumber] = brickToAdd;
			m_BrickCount++;
		}
		
		
	}
}

void bomberman::Grid::CreateGameObjects(dae::Scene& scene)
{
	for (int cell = 0; cell < static_cast<int>(m_Grid.size()); cell++)
	{
		switch (m_Grid[cell].cellType)
		{
		case CellTypes::Wall:
			break;
		case CellTypes::Brick:
			CreateBrick(scene, cell);
			break;
		case CellTypes::PlayerSpawn:
			break;
		case CellTypes::EnemySpawn:
			break;
		case CellTypes::Empty:
			break;
		default:
			break;
		}
	}
}

void bomberman::Grid::BrickDestroyed(int cellID)
{
	if (cellID < 0 or cellID >= static_cast<int>(m_Grid.size())) 
	{
		throw std::out_of_range("Cell ID out of range"); 
	}

	if (m_Grid[cellID].cellType != CellTypes::Brick)
	{
		throw std::invalid_argument("Cell ID is not a brick");
	}
	m_Grid[cellID].cellType = CellTypes::Empty;
	m_BrickCount--;

	if (m_BrickCount <= 0)
	{
		m_BrickCount = 0;
	}
}

bomberman::GridCell bomberman::Grid::GetCell(int cellID) const
{
	if (cellID >= static_cast<int>(m_Grid.size()) or cellID < 0) 
	{
		throw std::out_of_range("Cell number out of range");
	}

	return m_Grid[cellID];
}

bomberman::GridCell bomberman::Grid::GetCell(int column, int row) const
{
	return GetCell(GetCellID(column, row));
}

int bomberman::Grid::GetCellID(int column, int row) const
{
	return (row * TILES_AMOUNT_HORIZONTAL) + column;
}

void bomberman::Grid::CreateBrick(dae::Scene& scene, int gridID)
{
	GridCell sourceCell = m_Grid[gridID];

	auto go = std::make_shared<dae::GameObject>("BrickWall", GridCoordToWorldPos(sourceCell.column, sourceCell.row));
	go->AddComponent<dae::TextureComponent>(*go.get()).AddTexture("Brick.png");
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Brick, bomberman::Box(0.0f, 0.0f, TILE_SIZE, TILE_SIZE));
	go->AddComponent<bomberman::HealthComponent>(*go.get(), 1);
	scene.Add(go);
}
