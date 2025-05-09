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
		Enemy,
		Bomb,
		Brick,
		Player
	};

    class BaseCollider : public dae::Component
    {
	public:
		BaseCollider(dae::GameObject& gameObject, CollisionType collisionType = CollisionType::None);
		virtual ~BaseCollider();

		virtual void Update(float const deltaTime) override;
		virtual void Render() const override;
		virtual void RenderImGui() override;

		polygon GetHitBox() const { return m_Polygon; }

		bool IsOverlapping(polygon other, glm::vec3 otherPosition);

		void SetPolygon(polygon polygon) { m_Polygon = polygon; CalculateAxes(m_Polygon); }

		void ToggleRender() { m_ShouldRender = !m_ShouldRender; }

	protected:
		//Checks if this object should check for collisions
		bool ShouldCheckCollision() const;

		polygon CalculateAxes(polygon poly);
		void ProjectPolygon(const polygon& polygon, const glm::vec2& axis, float& min, float& max) const;
		bool OverlapOnAxis(const polygon& polyA, const polygon& polyB, const glm::vec2& axis) const;

		void AddPositionToPolygon(polygon& polygon, glm::vec3 position);

		void ResetMovement(BaseCollider* otherCollider);

		polygon m_Polygon;
		polygon m_Axes;
		CollisionType m_CollisionType;
		bool m_ShouldRender{ false };
    };
}

