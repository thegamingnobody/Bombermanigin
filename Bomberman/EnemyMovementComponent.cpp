#include "EnemyMovementComponent.h"
#include "EnemyManager.h"

bomberman::EnemyMovementComponent::EnemyMovementComponent(dae::GameObject& gameObject, EnemyType enemyType) 
	: dae::Component(gameObject)
	, m_EnemyType(enemyType)
{
	m_Speed = bomberman::EnemyManager::GetInstance().GetEnemySpeed(enemyType);
}
