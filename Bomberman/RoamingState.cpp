#include "RoamingState.h"
#include <iostream>
#include "Grid.h"
#include "EnemyManager.h"
#include <cmath>
#include <EventManager.h>
#include "EventTypes.h"
#include "EnemyCollisionEvent.h"

bomberman::RoamingState::RoamingState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
	, m_Direction(0.0f, 0.0f, 0.0f)
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::RoamingState::Update(float deltaTime)
{
	//Get variables
	//auto& grid = bomberman::Grid::GetInstance();
	auto& enemyManager = bomberman::EnemyManager::GetInstance();
	auto speed = enemyManager.GetEnemyData(bomberman::EnemyType::Balloom).speed;
	auto transform = m_Owner->GetTransform(); 

	//Calculate the movement this frame
	glm::vec3 deltaPosition{ m_Direction.x * speed * deltaTime, m_Direction.y * speed * deltaTime, 0.0f };
	transform->Move(deltaPosition);

	return nullptr;
}

void bomberman::RoamingState::OnEnter()
{
	int positiveOrNegative{ ((rand() % 2) * 2) - 1 };
	int randomDirection{ rand() % 2 };

	m_Direction.x = static_cast<float>(randomDirection * positiveOrNegative);
	m_Direction.y = (static_cast<int>(m_Direction.x) == 0) ? 1.0f * positiveOrNegative : 0.0f;

	dae::EventManager::GetInstance().AddObserver(*this, static_cast<int>(bomberman::EventType::ENEMY_COLLISION));
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
	// Todo: chance to change axis instead of just flipping
	m_Direction = {
		-m_Direction.y,
		m_Direction.x,
		0.0f
	};
}

