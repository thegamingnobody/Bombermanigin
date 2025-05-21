#include "ChaseState.h"
#include <iostream>
#include <SceneManager.h>
#include <Scene.h>
#include "RoamingState.h"
#include "Grid.h"
#include "EventTypes.h"
#include <EventManager.h>
#include "ObjectDamagedEvent.h"
#include "DyingState.h"
#include "StateMachineComponent.h"
#include "HealthComponent.h"

bomberman::ChaseState::ChaseState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::ChaseState::Update(float deltaTime)
{
	auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();
	if (healthComp.has_value() and healthComp.value()->GetCurrentHealth() <= 0)
	{
		return std::make_unique<bomberman::DyingState>(*m_Owner);
	}

	//Todo: improve
	auto player = dae::SceneManager::GetInstance().GetScene("Player")->GetObject("Player 1");
	if (!player)
	{
		throw std::runtime_error("Player not found");
	}
	auto transform = m_Owner->GetTransform();

	glm::vec3 playerPos = player->GetTransform()->GetGlobalPosition();
	glm::vec3 enemyPos = transform->GetGlobalPosition();

	glm::vec3 directionToPlayer = playerPos - enemyPos;
	float distanceSQ = (directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);

	//float distance = (playerPos - enemyPos);

	if (distanceSQ > m_EnemyData.detectionRange * m_EnemyData.detectionRange)
	{
		return std::make_unique<bomberman::RoamingState>(*m_Owner);
	}

	glm::vec3 directionToTest{0.0f, 0.0f, 0.0f};
	float const tolerance{ 2.0f };

	if (std::abs(directionToPlayer.x) < std::abs(directionToPlayer.y))
	{
		//Move vertical
		if (std::abs(directionToPlayer.x) <= tolerance)
		{
			directionToTest = glm::vec3(0.0f, 1.0f, 0.0f);
			directionToTest *= (directionToPlayer.y > 0.0f) ? 1.0f : -1.0f;
		}
		else
		{
			//make sure enemy doesn't move up and down when it can't/shouldn't
			directionToTest = glm::vec3(1.0f, 0.0f, 0.0f);
			directionToTest *= (directionToPlayer.x > 0.0f) ? 1.0f : -1.0f;
			directionToPlayer.y = 0.0f;
		}
	}
	else
	{
		//Move horizontal
		if (std::abs(directionToPlayer.y) <= tolerance)
		{
			directionToTest = glm::vec3(1.0f, 0.0f, 0.0f);
			directionToTest *= (directionToPlayer.x > 0.0f) ? 1.0f : -1.0f;
		}
		else
		{
			//make sure enemy doesn't move left and right when it can't/shouldn't
			directionToTest = glm::vec3(0.0f, 1.0f, 0.0f);
			directionToTest *= (directionToPlayer.y > 0.0f) ? 1.0f : -1.0f;
			directionToPlayer.x = 0.0f;
		}
	}
	
	auto test = IsDirectionValid(directionToTest);

	if (test == false) return nullptr;

	directionToPlayer = glm::vec3(1.0f * directionToTest.x, 1.0f * directionToTest.y, 0.0f);

	glm::vec3 deltaPosition{ directionToPlayer.x * m_EnemyData.speed * deltaTime, directionToPlayer.y * m_EnemyData.speed * deltaTime, 0.0f };
	transform->Move(deltaPosition);

    return nullptr;
}

void bomberman::ChaseState::OnEnter()
{
	//Get the enemy data from the state machine component
	auto stateMachineComponent = m_Owner->GetComponent<bomberman::StateMachineComponent>();
	if (stateMachineComponent.has_value())
	{
		m_EnemyData = stateMachineComponent.value()->GetEnemyData();
	}
}

void bomberman::ChaseState::OnExit()
{
}

bool bomberman::ChaseState::IsDirectionValid(glm::vec3 direction) const
{
	auto& grid = bomberman::Grid::GetInstance();
	glm::vec3 enemyPos = m_Owner->GetTransform()->GetGlobalPosition();

	glm::vec3 enemyCell = grid.WorldPosToGridCoord(enemyPos);

	bomberman::GridCell nextCell = bomberman::GridCell(static_cast<int>(enemyCell.x + direction.x), static_cast<int>(enemyCell.y + direction.y));

	auto nextCellID = grid.GetCell(grid.GetCellID(nextCell.column, nextCell.row));

	return grid.IsCellTypeWalkable(nextCell.cellType);	
}
