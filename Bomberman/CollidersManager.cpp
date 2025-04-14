#include "CollidersManager.h"

void bomberman::CollidersManager::AddCollider(bomberman::BoxCollider& collider)
{
	auto it = std::find(m_Colliders.begin(), m_Colliders.end(), &collider);

	if (it != m_Colliders.end()) return;

	m_Colliders.push_back(&collider);
}

void bomberman::CollidersManager::RemoveCollider(bomberman::BoxCollider& collider)
{
	auto it = std::find(m_Colliders.begin(), m_Colliders.end(), &collider);

	if (it == m_Colliders.end()) return;

	m_Colliders.erase(it);
}
