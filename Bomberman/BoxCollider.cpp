#include "BoxCollider.h"
#include <Renderer.h>
#include "CollidersManager.h"
#include "Grid.h"
#include <iostream>

bomberman::BoxCollider::BoxCollider(dae::GameObject& gameObject, CollisionType collisionType, Box hitBox)
	: BaseCollider(gameObject, collisionType)
{
	CollidersManager::GetInstance().AddCollider(*this);
	m_Polygon = CreateVerts(hitBox);
	m_Axes = CalculateAxes(m_Polygon);
}

void bomberman::BoxCollider::Update(float const /*deltaTime*/)
{
	if (!ShouldCheckCollision()) return;

	auto& collidersManager = CollidersManager::GetInstance();
	auto colliders = collidersManager.GetColliders();

	for (const auto& collider : colliders)
	{
		if (collider == this) continue;

		polygon other = collider->GetHitBox();
		auto otherPosition = collider->GetOwner()->GetTransform()->GetGlobalPosition();

		if (IsOverlapping(other, otherPosition))
		{
			std::cout << "Collision\n";
			GetOwner()->GetTransform()->ResetMovementThisFrame();
		}
	}
}

void bomberman::BoxCollider::Render() const
{
	glm::vec3 position = GetOwner()->GetTransform()->GetGlobalPosition();
	glm::vec2 size = { (m_Polygon[0] - m_Polygon[1]).length(), (m_Polygon[1] - m_Polygon[2]).length() };
	//glm::vec3 color = { 1.0f, 0.0f, 0.0f }; // Red color
	auto& renderer = dae::Renderer::GetInstance();

	for (int vert = 0; vert < m_Polygon.size(); vert++)
	{
		glm::vec2 p1 = m_Polygon[vert];
		glm::vec2 p2 = m_Polygon[(vert + 1) % m_Polygon.size()];
		glm::vec3 center = glm::vec3(position.x, position.y, position.z);
		renderer.RenderLine(center.x + p1.x, center.y + p1.y, center.x + p2.x, center.y + p2.y);
	}
}

//std::vector<glm::vec2> bomberman::BoxCollider::CreateOctagon(float x, float y, float size) const
//{
//	int const bevelInset{ 2 };
//	float cx = x + size / 2;
//	float cy = y + size / 2;
//
//	return {
//		{cx - bevelInset, cy - size / 2},
//		{cx + bevelInset, cy - size / 2},
//		{cx + size / 2, cy - bevelInset},
//		{cx + size / 2, cy + bevelInset},
//		{cx + bevelInset, cy + size / 2},
//		{cx - bevelInset, cy + size / 2},
//		{cx - size / 2, cy + bevelInset},
//		{cx - size / 2, cy - bevelInset}
//	};
//}

polygon bomberman::BoxCollider::CreateVerts(Box hitbox) const
{
	polygon result;

	result.emplace_back(hitbox.x, hitbox.y);
	result.emplace_back(hitbox.x + hitbox.width, hitbox.y);
	result.emplace_back(hitbox.x + hitbox.width, hitbox.y + hitbox.height);
	result.emplace_back(hitbox.x, hitbox.y + hitbox.height);

	return result;
}
