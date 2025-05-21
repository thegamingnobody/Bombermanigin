#include "Grid.h"
#include <json.hpp>
#include <fstream>
#include <TextureComponent.h>
#include "HealthComponent.h"
#include "BoxCollider.h"
#include "StateMachineComponent.h"
#include "BrickIdleState.h"


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

	for (int row = 2; row < TILES_AMOUNT_VERTICAL; row += 2)
	{
		for (int column = 2; column < TILES_AMOUNT_HORIZONTAL; column += 2)
		{
			m_Grid[column + row * TILES_AMOUNT_HORIZONTAL].cellType = CellTypes::Wall;
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

	//Path is different in the build folder, so we need to check both paths
	try
	{
		data = json::parse(f);
	}
	catch (...)
	{
		data = json::parse(f2);
	}

	if (levelID >= static_cast<int>(data.size()))
	{
		throw std::out_of_range("Level ID out of range");
	}

	// Add bricks to the grid
	m_LevelID = data[levelID]["level"];
	auto jsonBrick = data[levelID]["bricks"];
	for (int column = 1; column <= static_cast<int>(jsonBrick.size()); column++)
	{
		auto brick = jsonBrick[column - 1];
		for (int row = 0; row < static_cast<int>(brick.size()); row++)
		{
			GridCell brickToAdd = GridCell(column, brick[row], CellTypes::Brick);
			int cellNumber = GetCellID(column, row);

			if (IsCellValid(cellNumber) == false)
			{
				throw std::out_of_range("Cell number out of range");
			}

			m_Grid[cellNumber] = brickToAdd;
			m_BrickCount++;
		}
	}

	auto& enemyManager = bomberman::EnemyManager::GetInstance();
	auto& grid = bomberman::Grid::GetInstance();
	auto objectsScene = dae::SceneManager::GetInstance().GetScene("Objects");


	// Add enemy spawns to the grid
	auto jsonEnemySpawns = data[levelID]["enemySpawns"];
	for (int spawn = 0; spawn < static_cast<int>(jsonEnemySpawns.size()); spawn++)
	{
		auto position = jsonEnemySpawns[spawn];
		GridCell enemySpawnToAdd = GridCell(position[0], position[1], CellTypes::EnemySpawn);
		int cellNumber = GetCellID(enemySpawnToAdd.column, enemySpawnToAdd.row);

		if (IsCellValid(cellNumber) == false)
		{
			throw std::out_of_range("Cell number out of range");
		}

		m_Grid[cellNumber] = enemySpawnToAdd;

		auto go = enemyManager.CreateEnemy(bomberman::EnemyType(data[levelID]["enemyTypes"][0]), grid.GridCoordToWorldPos(enemySpawnToAdd));
		objectsScene->Add(go);
	}
}

void bomberman::Grid::CreateGameObjects(dae::Scene& scene)
{
	// Create the game objects based on the grid data
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

bool bomberman::Grid::IsCellTypeWalkable(CellTypes cellType) const
{
	return cellType == CellTypes::Empty || cellType == CellTypes::PlayerSpawn || cellType == CellTypes::EnemySpawn;
}

bool bomberman::Grid::IsCellValid(int cellID) const
{
	return !(cellID >= static_cast<int>(m_Grid.size()) or cellID < 0);
}

void bomberman::Grid::CreateBrick(dae::Scene& scene, int gridID)
{
	GridCell sourceCell = m_Grid[gridID];

	auto go = std::make_shared<dae::GameObject>("BrickWall", GridCoordToWorldPos(sourceCell.column, sourceCell.row));
	go->AddComponent<dae::TextureComponent>(*go.get()).AddTexture("Brick.png");
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Brick, bomberman::Box(0.0f, 0.0f, TILE_SIZE, TILE_SIZE));
	go->AddComponent<bomberman::HealthComponent>(*go.get(), 1);

	//Giving the bricks their own state machine is maybe a bit redundant,
	//but it makes everything more uniform (and a destruction animation would be easier this way)
	auto& statemachine = go->AddComponent<bomberman::StateMachineComponent>(*go.get());
	std::unique_ptr<bomberman::BrickIdleState> idleState = std::make_unique<bomberman::BrickIdleState>(*go.get());
	statemachine.ChangeState(std::move(idleState));

	scene.Add(go);
}
