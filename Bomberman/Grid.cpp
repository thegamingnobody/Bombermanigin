#include "Grid.h"
#include <json.hpp>
#include <fstream>
#include <TextureComponent.h>
#include "HealthComponent.h"
#include "BoxCollider.h"
#include "StateMachineComponent.h"
#include "BrickIdleState.h"
#include "GameManager.h"

using json = nlohmann::json;

void bomberman::Grid::Init()
{
}

bomberman::LevelData bomberman::Grid::LoadMap(int const levelID)
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

	LoadStaticMap();

	LevelData levelData = LevelData();
	levelData.levelNumber = levelID+1;

	// Add bricks to the grid
	auto jsonBrick = data[levelID]["bricks"];
	for (int column = 1; column <= static_cast<int>(jsonBrick.size()); column++)
	{
		auto brick = jsonBrick[column - 1];
		for (int row = 0; row < static_cast<int>(brick.size()); row++)
		{
			GridCell brickToAdd = GridCell(column, brick[row], CellTypes::Brick);
			int cellNumber = GetCellID(brickToAdd.column, brickToAdd.row);

			if (IsCellValid(cellNumber) == false)
			{
				throw std::out_of_range("Cell number out of range");
			}

			m_Grid[cellNumber] = brickToAdd;
			m_BrickCount++;
			levelData.brickWalls.emplace_back(brickToAdd);
		}
	}

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
		levelData.enemySpawns.emplace_back(enemySpawnToAdd);
	}

	auto jsonEnemyTypes = data[levelID]["enemyTypes"];
	for (int enemyType = 0; enemyType < static_cast<int>(jsonEnemyTypes.size()); enemyType++)
	{
		auto type = jsonEnemyTypes[enemyType];
		levelData.enemyTypes.emplace_back(type);
	}

	// Add player spawns to the grid
	auto playerSpawn = GridCell(1, 1, CellTypes::PlayerSpawn);
	m_Grid[GetCellID(1, 1)] = playerSpawn;
	levelData.playerSpawns.emplace_back(playerSpawn);

	return levelData;
}

void bomberman::Grid::LoadMap(LevelData const& levelData)
{
	//Initialize
	m_BrickCount = 0;
	m_Grid.clear();

	LoadStaticMap();

	for (const auto& brick : levelData.brickWalls)
	{
		int cellNumber = GetCellID(brick.column, brick.row);
		if (IsCellValid(cellNumber) == false)
		{
			throw std::out_of_range("Cell number out of range");
		}
		m_Grid[cellNumber] = brick;
		m_BrickCount++;
	}

	for (const auto& enemySpawn : levelData.enemySpawns)
	{
		int cellNumber = GetCellID(enemySpawn.column, enemySpawn.row);
		if (IsCellValid(cellNumber) == false)
		{
			throw std::out_of_range("Cell number out of range");
		}
		m_Grid[cellNumber] = enemySpawn;
	}

}

void bomberman::Grid::CreateGameObjects()
{
	// Create the game objects based on the grid data
	for (int cell = 0; cell < static_cast<int>(m_Grid.size()); cell++)
	{
		switch (m_Grid[cell].cellType)
		{
		case CellTypes::Wall:
			break;
		case CellTypes::Brick:
			CreateBrick(cell);
			break;
		case CellTypes::PlayerSpawn:
			break;
		case CellTypes::EnemySpawn:
			CreateEnemy(cell);
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

	if (m_Grid[cellID].cellType != CellTypes::Brick) return;

	m_Grid[cellID].cellType = CellTypes::Empty;
	m_BrickCount--;

	if (m_BrickCount <= 0)
	{
		m_BrickCount = 0;
	}

	if (m_DoorFound) return;

	if (rand() % m_BrickCount == 0 or m_BrickCount == 0)
	{
		m_DoorFound = true;
		m_Grid[cellID].cellType = CellTypes::Door;
		CreateDoor(cellID);
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

void bomberman::Grid::LoadStaticMap()
{
	m_Grid.resize(TILES_AMOUNT_HORIZONTAL * TILES_AMOUNT_VERTICAL);
	
	// Outer walls
	for (int row = 0; row < TILES_AMOUNT_VERTICAL; row++)
	{
		for (int column = 0; column < TILES_AMOUNT_HORIZONTAL; column++)
		{
			GridCell newCell = GridCell(column, row, (column == 0 or row == 0 or column + 1 == TILES_AMOUNT_HORIZONTAL or row + 1 == TILES_AMOUNT_VERTICAL) ? CellTypes::Wall : CellTypes::Empty);
			m_Grid[column + row * TILES_AMOUNT_HORIZONTAL] = newCell;
		}
	}

	// Inner walls
	for (int row = 2; row < TILES_AMOUNT_VERTICAL; row += 2)
	{
		for (int column = 2; column < TILES_AMOUNT_HORIZONTAL; column += 2)
		{
			m_Grid[column + row * TILES_AMOUNT_HORIZONTAL].cellType = CellTypes::Wall;
		}
	}

}

void bomberman::Grid::CreateBrick(int gridID)
{
	GridCell sourceCell = m_Grid[gridID];
	auto objectsScene = dae::SceneManager::GetInstance().GetScene("Objects");

	auto go = std::make_shared<dae::GameObject>("BrickWall", GridCoordToWorldPos(sourceCell.column, sourceCell.row));
	go->AddComponent<dae::TextureComponent>(*go.get()).AddTexture("Brick.png");
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Brick, bomberman::Box(0.0f, 0.0f, TILE_SIZE, TILE_SIZE));
	go->AddComponent<bomberman::HealthComponent>(*go.get(), 1);

	//Giving the bricks their own state machine is maybe a bit redundant,
	//but it makes everything more uniform (and a destruction animation would be easier this way)
	auto& statemachine = go->AddComponent<bomberman::StateMachineComponent>(*go.get());
	std::unique_ptr<bomberman::BrickIdleState> idleState = std::make_unique<bomberman::BrickIdleState>(*go.get());
	statemachine.ChangeState(std::move(idleState));

	objectsScene->Add(go);
}

void bomberman::Grid::CreateEnemy(int gridID)
{
	auto& enemyManager = bomberman::EnemyManager::GetInstance();
	auto& gameManager = bomberman::GameManager::GetInstance();
	auto objectsScene = dae::SceneManager::GetInstance().GetScene("Objects");
	auto levelData = gameManager.GetLevelData();

	auto go = enemyManager.CreateEnemy(bomberman::EnemyType(levelData.enemyTypes[0]), GridCoordToWorldPos(GetCell(gridID)));
	objectsScene->Add(go);
}

void bomberman::Grid::CreateDoor(int gridID)
{
	GridCell sourceCell = m_Grid[gridID];
	auto objectsScene = dae::SceneManager::GetInstance().GetScene("Objects");

	auto go = std::make_shared<dae::GameObject>("Door", GridCoordToWorldPos(sourceCell.column, sourceCell.row));
	go->AddComponent<dae::TextureComponent>(*go.get()).AddTexture("Door.png");
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Door, bomberman::Box(0.0f, 0.0f, TILE_SIZE, TILE_SIZE));

	objectsScene->Add(go);
}
