#pragma once
#include <Component.h>
#include <vector>
#include <glm.hpp>

using polygon = std::vector<glm::vec2>;

namespace bomberman
{
	enum class CollisionType
	{
		None,
		Wall,
		Entity
	};

    class BaseCollider : public dae::Component
    {
	public:
		BaseCollider(dae::GameObject& gameObject, CollisionType collisionType = CollisionType::None);
		virtual ~BaseCollider() override = default;

		virtual void Update(float const deltaTime) override = 0;
		virtual void Render() const override = 0;

		polygon GetHitBox() const { return m_Polygon; }

		bool IsOverlapping(polygon other, glm::vec3 otherPosition);

		void SetPolygon(polygon polygon) { m_Polygon = polygon; CalculateAxes(m_Polygon); }

	protected:
		//Checks if this object should check for collisions
		bool ShouldCheckCollision() const;

		polygon CalculateAxes(polygon poly);
		void ProjectPolygon(const polygon& polygon, const glm::vec2& axis, float& min, float& max) const;
		bool OverlapOnAxis(const polygon& polyA, const polygon& polyB, const glm::vec2& axis) const;

		void AddPositionToPolygon(polygon& polygon, glm::vec3 position);

		polygon m_Polygon;
		polygon m_Axes;
		CollisionType m_CollisionType;
    };
}

