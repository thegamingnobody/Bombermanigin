#include "CollidersManager.h"

void bomberman::CollidersManager::Init()
{
	// Initialize collision handlers
	CollisionKey playerWallKey{ CollisionType::Player, CollisionType::Wall };
	SetCollisionHandler(playerWallKey.sourceType, playerWallKey.otherType, [](BaseCollider* self, BaseCollider* other)
		{
			self->ResetMovement(other);
		});

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

void bomberman::CollidersManager::SetCollisionHandler(bomberman::CollisionType sourceType, bomberman::CollisionType otherType, CollisionHandler handler)
{
	CollisionKey keyToAdd{ sourceType, otherType };
	m_CollisionHandlers[keyToAdd] = handler;
	m_CollisionHandlers[keyToAdd.Reverse()] = [&](BaseCollider* self, BaseCollider* other)
	{
		m_CollisionHandlers[keyToAdd](other, self);
	};
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
