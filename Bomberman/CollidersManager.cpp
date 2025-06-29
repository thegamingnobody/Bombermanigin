#include "CollidersManager.h"
#include "EnemyCollisionEvent.h"
#include "EventManager.h"
#include "HealthComponent.h"
#include "GameManager.h"
#include "EnemyManager.h"
#include "PickupComponent.h"
#include "PlayerManager.h"
#include "BombComponent.h"

void bomberman::CollidersManager::Init()
{
	// Initialize collision handlers
	CollisionHandler stopMovement = [](BaseCollider* self, BaseCollider* other)
		{
		self->ResetMovement(other);
		};
	CollisionHandler stopMovementEnemy = [](BaseCollider* self, BaseCollider* other)
		{
		bomberman::EnemyCollisionEvent event{ self->GetOwner()->GetName(), self->GetCollisionType() };
		dae::EventManager::GetInstance().BroadcastEvent(std::move(std::make_unique<bomberman::EnemyCollisionEvent>(event)));
		self->ResetMovement(other);
		};
	CollisionHandler damageOtherCollider = [](BaseCollider* /*self*/, BaseCollider* other)
		{
			auto healthComponent{ other->GetOwner()->GetComponent<bomberman::HealthComponent>() };
			if (healthComponent.has_value())
			{
				healthComponent.value()->Damage(1);
			}
		};
	CollisionHandler tryNextLevel = [](BaseCollider* /*self*/, BaseCollider* /*other*/)
		{
			if (bomberman::EnemyManager::GetInstance().AreAllEnemiesDead())
			{
				bomberman::GameManager::GetInstance().LoadNextLevel();
			}
		};

	CollisionHandler CollectPickUp = [](BaseCollider* self, BaseCollider* other)
		{
			auto powerUp = other->GetOwner()->GetComponent<bomberman::PickupComponent>();
			if (powerUp.has_value())
			{
				std::string playerName = self->GetOwner()->GetName();

				PlayerInfo playerInfo = bomberman::PlayerManager::GetInstance().GetPlayerInfo(playerName);
				powerUp.value()->CollectPickup(playerInfo.playerID);
				other->GetOwner()->SetShouldBeRemoved();
			}
		};

	CollisionHandler ExplodeBomb = [](BaseCollider* /*self*/, BaseCollider* other)
		{
			auto bomb = other->GetOwner()->GetComponent<bomberman::BombComponent>();
			if (bomb.has_value())
			{
				bomb.value()->Explode();
			}
		};

	SetCollisionHandler(CollisionType::Player, CollisionType::Wall, stopMovement, false);
	SetCollisionHandler(CollisionType::Player, CollisionType::Brick, stopMovement, false);
	SetCollisionHandler(CollisionType::Enemy, CollisionType::Wall, stopMovementEnemy, false);
	SetCollisionHandler(CollisionType::Enemy, CollisionType::Brick, stopMovementEnemy, false);
	SetCollisionHandler(CollisionType::Enemy, CollisionType::Bomb, stopMovementEnemy, false);

	SetCollisionHandler(CollisionType::Enemy, CollisionType::Player, damageOtherCollider, false);
	SetCollisionHandler(CollisionType::Explosion, CollisionType::Brick, damageOtherCollider, false);
	SetCollisionHandler(CollisionType::Explosion, CollisionType::Enemy, damageOtherCollider, false);
	SetCollisionHandler(CollisionType::Explosion, CollisionType::Player, damageOtherCollider, false);

	SetCollisionHandler(CollisionType::Explosion, CollisionType::Bomb, ExplodeBomb, false);

	SetCollisionHandler(CollisionType::Player, CollisionType::Door, tryNextLevel, false);

	SetCollisionHandler(CollisionType::Player, CollisionType::PickUp, CollectPickUp, false);
}

void bomberman::CollidersManager::AddCollider(bomberman::BaseCollider& collider)
{
	auto it = std::find(m_Colliders.begin(), m_Colliders.end(), &collider);

	if (it != m_Colliders.end()) return;

	m_Colliders.push_back(&collider);
}

void bomberman::CollidersManager::RemoveCollider(bomberman::BaseCollider& collider)
{
	auto it = std::find(m_Colliders.begin(), m_Colliders.end(), &collider);

	if (it == m_Colliders.end()) return;

	m_Colliders.erase(it);
}

void bomberman::CollidersManager::SetCollisionHandler(bomberman::CollisionType sourceType, bomberman::CollisionType otherType, CollisionHandler handler, bool alsoSetReverse)
{
	CollisionKey keyToAdd{ sourceType, otherType };
	m_CollisionHandlers[keyToAdd] = handler;
	if (alsoSetReverse)
	{
		m_CollisionHandlers[keyToAdd.Reverse()] = [&](BaseCollider* self, BaseCollider* other)
		{
			m_CollisionHandlers[keyToAdd](other, self);
		};
	}
}

void bomberman::CollidersManager::HandleCollision(bomberman::BaseCollider* self, bomberman::BaseCollider* other)
{
	CollisionKey key{ self->GetCollisionType(), other->GetCollisionType() };

	auto it = m_CollisionHandlers.find(key);
	if (it != m_CollisionHandlers.end())
	{
		it->second(self, other);
	}
}

bool bomberman::CollidersManager::IsSourceType(bomberman::CollisionType sourceType) const
{
	for (const auto& key : m_CollisionHandlers)
	{
		if (key.first.sourceType == sourceType)
		{
			return true;
		}
	}

	return false;
}

bool bomberman::CollidersManager::IsKeySet(CollisionKey key) const
{
	auto it = m_CollisionHandlers.find(key);

	return (it != m_CollisionHandlers.end());
}
