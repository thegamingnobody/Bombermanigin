#include "BaseCollider.h"
#include "CollidersManager.h"

bomberman::BaseCollider::BaseCollider(dae::GameObject& gameObject, CollisionType collisionType)
	: dae::Component(gameObject)
	, m_CollisionType(collisionType)
{
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

