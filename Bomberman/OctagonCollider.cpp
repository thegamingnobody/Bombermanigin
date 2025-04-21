#include "OctagonCollider.h"
#include "CollidersManager.h"
#include "Grid.h"

bomberman::OctagonCollider::OctagonCollider(dae::GameObject& gameObject, CollisionType collisionType, float bevelInset)
	: BaseCollider(gameObject, collisionType)
	, m_BevelInset(bevelInset)
{
	CollidersManager::GetInstance().AddCollider(*this);
	m_Polygon = CreateOctagon(0.0f, 0.0f);
	m_Axes = CalculateAxes(m_Polygon);
}

polygon bomberman::OctagonCollider::CreateOctagon(float x, float y) const
{
	float cx = x + TILE_SIZE / 2;
	float cy = y + TILE_SIZE / 2;

	return {
		{cx - m_BevelInset, cy - TILE_SIZE / 2},
		{cx + m_BevelInset, cy - TILE_SIZE / 2},
		{cx + TILE_SIZE / 2, cy - m_BevelInset},
		{cx + TILE_SIZE / 2, cy + m_BevelInset},
		{cx + m_BevelInset, cy + TILE_SIZE / 2},
		{cx - m_BevelInset, cy + TILE_SIZE / 2},
		{cx - TILE_SIZE / 2, cy + m_BevelInset},
		{cx - TILE_SIZE / 2, cy - m_BevelInset}
	};
}

