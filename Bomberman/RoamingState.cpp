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
		return std::make_unique<bomberman::DyingState>(*m_Owner);
	}

	//Get variables
	auto transform = m_Owner->GetTransform(); 

	//Calculate the movement this frame
	glm::vec3 deltaPosition{ m_Direction.x * m_EnemyData.speed * deltaTime, m_Direction.y * m_EnemyData.speed * deltaTime, 0.0f };
	transform->Move(deltaPosition);

	//Todo: improve
	auto player = dae::SceneManager::GetInstance().GetScene(SCENE_PLAYERS)->GetObject("Player 1");
	if (!player)
	{
		throw std::runtime_error("Player not found");
	}

	glm::vec3 playerPos = player->GetTransform()->GetGlobalPosition();

	glm::vec3 enemyPos = m_Owner->GetTransform()->GetGlobalPosition();

	float distanceSQ =	(playerPos.x - enemyPos.x) * (playerPos.x - enemyPos.x) + 
						(playerPos.y - enemyPos.y) * (playerPos.y - enemyPos.y);

	//float distance = (playerPos - enemyPos);

	if (distanceSQ <= m_EnemyData.detectionRange * m_EnemyData.detectionRange)
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

	//Subscribe to event
	eventManager.AddObserver(*this, static_cast<int>(bomberman::EventType::ENEMY_COLLISION));
}

void bomberman::RoamingState::OnExit()
{
	dae::EventManager::GetInstance().RemoveObserver(*this);
}

void bomberman::RoamingState::Notify(const dae::Event& event)
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

