#pragma once
#include <Component.h>
#include "EnemyManager.h"

namespace bomberman
{
    class EnemyMovementComponent : public dae::Component
    {
	public:
		EnemyMovementComponent(dae::GameObject& gameObject, EnemyType enemyType);
		~EnemyMovementComponent() override = default;

		//void Update(float deltaTime) override;

	private:
		EnemyType m_EnemyType;
		int m_Speed;
		bool m_ChasePlayer{ false };
    };
}

