#pragma once
#include <Event.h>
#include "EventTypes.h"
#include <GameObject.h>
#include <string>
#include "BaseCollider.h"

namespace bomberman
{

    class EnemyCollisionEvent : public dae::Event
    {
	public:
		EnemyCollisionEvent(std::string enemyName, bomberman::CollisionType collisionType)
			: Event(static_cast<int>(bomberman::EventType::ENEMY_COLLISION))
			, m_EnemyName(enemyName)
			, m_CollisionType(collisionType)
		{
		}

		std::string GetEnemyName() const { return m_EnemyName; }

		bomberman::CollisionType GetCollisionType() const { return m_CollisionType; }

	private:
		std::string m_EnemyName;
		bomberman::CollisionType m_CollisionType;
    };

}