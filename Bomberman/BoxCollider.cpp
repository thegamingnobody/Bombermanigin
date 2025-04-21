#include "BoxCollider.h"
#include "CollidersManager.h"
#include "Grid.h"
#include <GameObject.h>

bomberman::BoxCollider::BoxCollider(dae::GameObject& gameObject, CollisionType collisionType, Box hitBox)
	: BaseCollider(gameObject, collisionType)
{
	CollidersManager::GetInstance().AddCollider(*this);
	m_Polygon = CreateVerts(hitBox);
	m_Axes = CalculateAxes(m_Polygon);
}

polygon bomberman::BoxCollider::CreateVerts(Box hitbox) const
{
	polygon result;

	result.emplace_back(hitbox.x, hitbox.y);
	result.emplace_back(hitbox.x + hitbox.width, hitbox.y);
	result.emplace_back(hitbox.x + hitbox.width, hitbox.y + hitbox.height);
	result.emplace_back(hitbox.x, hitbox.y + hitbox.height);

	return result;
}
