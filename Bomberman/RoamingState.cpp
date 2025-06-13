#include "RoamingState.h"
#include <iostream>
#include <cmath>
#include <EventManager.h>
#include "EventTypes.h"
#include "EnemyCollisionEvent.h"
#include "ChaseState.h"
#include "Grid.h"
#include "ObjectDamagedEvent.h"
#include "DyingState.h"
#include "StateMachineComponent.h"
#include "HealthComponent.h"
#include "SceneNames.h"

bomberman::RoamingState::RoamingState(dae::GameObject& ownerObject, const bomberman::EnemyData& enemyData)
	: StateMachineBase(ownerObject)
	, m_Direction(0.0f, 0.0f, 0.0f)
	, m_EnemyData(enemyData)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::RoamingState::Update(float deltaTime)
{
	auto healthComp = m_Owner->GetComponent<bomberman::HealthComponent>();
	if (healthComp.has_value() and healthComp.value()->GetCurrentHealth() <= 0)
	{
		return std::make_unique<bomberman::DyingState>(*m_Owner, m_EnemyData.score);
	}

	//Get variables
	auto transform = m_Owner->GetTransform(); 

	//Calculate the movement this frame
	glm::vec3 deltaPosition{ m_Direction.x * m_EnemyData.speed * deltaTime, m_Direction.y * m_EnemyData.speed * deltaTime, 0.0f };
	transform->Move(deltaPosition);

	auto scene = dae::SceneManager::GetInstance().GetScene(SCENE_PLAYERS);
	auto player1 = scene->GetObject("Player 1");
	auto player2 = scene->GetObject("Player 2");

	if (!player1 && !player2)
	{
		throw std::runtime_error("No players found");
	}

	dae::GameObject* closestPlayer = nullptr;
	float closestDistSq = std::numeric_limits<float>::max();
	glm::vec3 enemyPos = transform->GetGlobalPosition();

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

	if (closestPlayer)
	{
		return std::make_unique<bomberman::ChaseState>(*m_Owner, m_EnemyData);
	}

	return nullptr;
}

void bomberman::RoamingState::OnEnter()
{
	auto& eventManager = dae::EventManager::GetInstance();

	//Generate a random direction
	int positiveOrNegative{ ((rand() % 2) * 2) - 1 };
	int randomDirection{ rand() % 2 };

	m_Direction.x = static_cast<float>(randomDirection * positiveOrNegative);
	m_Direction.y = (static_cast<int>(m_Direction.x) == 0) ? 1.0f * positiveOrNegative : 0.0f;

	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();

	if (!stateMachineComp.has_value())
	{
		throw std::runtime_error("StateMachineComponent not found on owner object");
	}
	//Subscribe to event
	eventManager.AddObserver(*stateMachineComp.value(), static_cast<int>(bomberman::EventType::ENEMY_COLLISION));
}

void bomberman::RoamingState::OnExit()
{
	auto stateMachineComp = m_Owner->GetComponent<bomberman::StateMachineComponent>();

	dae::EventManager::GetInstance().RemoveObserver(*stateMachineComp.value());
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::RoamingState::Notify(const dae::Event& event)
{
	switch (static_cast<bomberman::EventType>(event.GetEventType()))
	{
	case bomberman::EventType::ENEMY_COLLISION:
	{
		const auto& castedEvent = dynamic_cast<const bomberman::EnemyCollisionEvent&>(event);

		if (castedEvent.GetEnemyName() == m_Owner->GetName())
		{
			m_Owner->GetTransform()->Move(-m_Direction);
			FlipDirection();
		}
		break;
	}
	default:
		break;
	}

	return nullptr;
}

void bomberman::RoamingState::FlipDirection()
{  

	//the chance is 1/odds
	int odds{ 3 };
	
	if (rand() % odds == 0)
	{
		m_Direction = {
			-m_Direction.y,
			m_Direction.x,
			0.0f
		};
		return;
	}

	m_Direction = {
		-m_Direction.x,
		-m_Direction.y,
		0.0f
	};
	
}

