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
#include "SceneNames.h"

bomberman::ChaseState::ChaseState(dae::GameObject& ownerObject, const bomberman::EnemyData& enemyData)
	: StateMachineBase(ownerObject)
	, m_EnemyData(enemyData)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::ChaseState::Update(float deltaTime)
{
	auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();
	if (healthComp.has_value() and healthComp.value()->GetCurrentHealth() <= 0)
	{
		return std::make_unique<bomberman::DyingState>(*m_Owner, m_EnemyData.score);
	}

	auto scene = dae::SceneManager::GetInstance().GetScene(SCENE_PLAYERS);
	auto player1 = scene->GetObject("Player 1");
	auto player2 = scene->GetObject("Player 2");

	if (!player1 && !player2)
	{
		throw std::runtime_error("No players found");
	}
	dae::GameObject* closestPlayer{};
	float closestDistSq = std::numeric_limits<float>::max();
	glm::vec3 enemyPos = m_Owner->GetTransform()->GetGlobalPosition();
	
	auto tryPlayer = [&](dae::GameObject* player) 
		{
			if (!player) return;

			glm::vec3 pos = player->GetTransform()->GetGlobalPosition();
			glm::vec3 diff = pos - enemyPos;
			float distSq = diff.x * diff.x + diff.y * diff.y;

			if (distSq <= m_EnemyData.detectionRange * m_EnemyData.detectionRange && distSq < closestDistSq)
			{
				closestPlayer = player;
				closestDistSq = distSq;
			}
		};

	tryPlayer(player1.get());
	tryPlayer(player2.get());

	if (!closestPlayer)
	{
		return std::make_unique<bomberman::RoamingState>(*m_Owner, m_EnemyData);
	}

	auto transform = m_Owner->GetTransform();

	glm::vec3 playerPos = closestPlayer->GetTransform()->GetGlobalPosition();

	glm::vec3 directionToPlayer = playerPos - enemyPos;

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
}

void bomberman::ChaseState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::ChaseState::Notify(const dae::Event& /*event*/)
{
	return nullptr;
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
