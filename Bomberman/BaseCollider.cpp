#include "BaseCollider.h"
#include "CollidersManager.h"
#include <Renderer.h>
#include <iostream>


bomberman::BaseCollider::BaseCollider(dae::GameObject& gameObject, CollisionType collisionType)
	: dae::Component(gameObject)
	, m_CollisionType(collisionType)
{
}

void bomberman::BaseCollider::Update(float const /*deltaTime*/)
{
	if (!ShouldCheckCollision()) return;

	auto& collidersManager = CollidersManager::GetInstance();
	auto colliders = collidersManager.GetColliders();

	auto& camera = dae::Camera::GetInstance();
	float tileScale{ camera.GetWindowScale() };

	for (const auto& collider : colliders)
	{
		if (collider == this) continue;

		auto otherPosition = collider->GetOwner()->GetTransform()->GetGlobalPosition();

		if (IsOverlapping(collider->GetHitBox(), otherPosition))
		{
			auto transform = GetOwner()->GetTransform();
			auto direction = transform->GetMovementThisFrame();

			float tileStep{ 1.0f * tileScale };

			if (static_cast<int>(direction.x) != 0)
			{
				transform->Move(glm::vec2(0.0f, -2*tileStep));
				if (!IsOverlapping(collider->GetHitBox(), otherPosition))
				{
					transform->Move(glm::vec2(0.0f, tileStep));
					continue;
				}

				transform->Move(glm::vec2(0.0f, 4.0f * tileStep));
				if (!IsOverlapping(collider->GetHitBox(), otherPosition))
				{
					transform->Move(glm::vec2(0.0f, -1.0f * tileStep));
					continue;
				}
			}
			else if (static_cast<int>(direction.y) != 0)
			{
				transform->Move(glm::vec2(-2*tileStep, 0.0f));
				if (!IsOverlapping(collider->GetHitBox(), otherPosition))
				{
					transform->Move(glm::vec2(tileStep, 0.0f));
					continue;
				}

				transform->Move(glm::vec2(4.0f * tileStep, 0.0f));
				if (!IsOverlapping(collider->GetHitBox(), otherPosition))
				{
					transform->Move(glm::vec2(-1.0f * tileStep, 0.0f));
					continue;
				}
			}
			
			transform->ResetMovementThisFrame();
		}
	}
}

void bomberman::BaseCollider::Render() const
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

bool bomberman::BaseCollider::IsOverlapping(polygon other, glm::vec3 otherPosition)
{
	polygon allAxes = m_Axes;
	polygon otherPolyAxes = CalculateAxes(other);
	allAxes.insert(allAxes.end(), otherPolyAxes.begin(), otherPolyAxes.end());

	auto position = GetOwner()->GetTransform()->GetPredictedPosition();
	polygon tempThisPoly = m_Polygon;
	AddPositionToPolygon(tempThisPoly, position);
	AddPositionToPolygon(other, otherPosition);

	for (auto axis : allAxes)
	{
		if (!OverlapOnAxis(tempThisPoly, other, axis))
		{
			return false;
		}
	}

	return true;
}

bool bomberman::BaseCollider::ShouldCheckCollision() const
{
	//Todo: Add collision type functionality
	switch (m_CollisionType)
	{
	case bomberman::CollisionType::None:
		return false;
	case bomberman::CollisionType::Wall:
		return false;
	case bomberman::CollisionType::Entity:
		return true;
	default:
		return false;
	}
}

polygon bomberman::BaseCollider::CalculateAxes(polygon poly)
{	
	polygon axes;

	for (size_t i = 0; i < poly.size(); i++)
	{
		glm::vec2 p1 = poly[i];
		glm::vec2 p2 = poly[(i + 1) % poly.size()];
		glm::vec2 edge = p2 - p1;
		edge = glm::vec2{ -edge.y, edge.x };
		glm::vec2 normal = glm::normalize(edge);
		axes.emplace_back(normal);
	}

	return axes;
}

bool bomberman::BaseCollider::OverlapOnAxis(const polygon& polyA, const polygon& polyB, const glm::vec2& axis) const
{
	float minA, maxA, minB, maxB;
	ProjectPolygon(polyA, axis, minA, maxA);
	ProjectPolygon(polyB, axis, minB, maxB);
	return !(maxA < minB || maxB < minA);
}

void bomberman::BaseCollider::AddPositionToPolygon(polygon& polygon, glm::vec3 position)
{
	for (auto& vert : polygon)
	{
		vert.x += position.x;
		vert.y += position.y;
	}
}

void bomberman::BaseCollider::ProjectPolygon(const polygon& polygon, const glm::vec2& axis, float& min, float& max) const
{
	min = max = glm::dot(axis, polygon[0]);
	for (size_t i = 1; i < polygon.size(); i++) 
	{
		float proj = glm::dot(axis, polygon[i]);
		if (proj < min) min = proj;
		if (proj > max) max = proj;
	}
}

