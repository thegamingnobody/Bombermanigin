#include "BaseCollider.h"
#include "CollidersManager.h"
#include <Renderer.h>
#include <iostream>
#include "..\3rdParty\Imgui\imgui.h"
#include "HealthComponent.h"
#include <EventManager.h>
#include "StateMachineComponent.h"
#include "EnemyCollisionEvent.h"

bomberman::BaseCollider::BaseCollider(dae::GameObject& gameObject, CollisionType collisionType)
	: dae::Component(gameObject)
	, m_CollisionType(collisionType)
{
}

bomberman::BaseCollider::~BaseCollider()
{
	auto& collidersManager = CollidersManager::GetInstance();
	collidersManager.RemoveCollider(*this);
}

void bomberman::BaseCollider::Update(float const /*deltaTime*/)
{
	if (!ShouldCheckCollision()) return;

	auto& collidersManager = CollidersManager::GetInstance();
	auto colliders = collidersManager.GetColliders();

	for (const auto& collider : colliders)
	{
		// Don't check collision with self
		if (collider == this) continue;

		// Don't check collision if the key pair is not registered
		if (!collidersManager.IsKeySet(CollisionKey(m_CollisionType, collider->m_CollisionType))) continue;

		// Check and handle collision
		auto otherPosition = collider->GetOwner()->GetTransform()->GetGlobalPosition();
		if (IsOverlapping(collider->GetHitBox(), otherPosition))
		{
			collidersManager.HandleCollision(this, collider);
		}
	}
}

void bomberman::BaseCollider::Render() const
{
	if (!m_ShouldRender) return;

	glm::vec3 position = GetOwner()->GetTransform()->GetGlobalPosition();
	glm::vec2 size = { (m_Polygon[0] - m_Polygon[1]).length(), (m_Polygon[1] - m_Polygon[2]).length() };
	//glm::vec3 color = { 1.0f, 0.0f, 0.0f }; // Red color
	auto& renderer = dae::Renderer::GetInstance();

	for (int vert = 0; vert < static_cast<int>(m_Polygon.size()); vert++)
	{
		glm::vec2 p1 = m_Polygon[vert];
		glm::vec2 p2 = m_Polygon[(vert + 1) % m_Polygon.size()];
		glm::vec3 center = glm::vec3(position.x, position.y, position.z);
		renderer.RenderLine(center.x + p1.x, center.y + p1.y, center.x + p2.x, center.y + p2.y);
	}
}

void bomberman::BaseCollider::RenderImGui()
{
	//ImGui::Begin("Colliders");
	//ImGui::Checkbox("Render Collider", &m_ShouldRender);
	//ImGui::End();
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
	return CollidersManager::GetInstance().IsSourceType(m_CollisionType);
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

void bomberman::BaseCollider::ResetMovement(BaseCollider* otherCollider)
{
	auto& camera = dae::Camera::GetInstance();
	float tileScale{ camera.GetWindowScale() };

	auto transform = GetOwner()->GetTransform();
	auto direction = transform->GetMovementThisFrame();

	auto otherPosition = otherCollider->GetOwner()->GetTransform()->GetGlobalPosition();

	float tileStep{ 1.0f * tileScale };

	if (static_cast<int>(direction.x) != 0)
	{
		transform->Move(glm::vec2(0.0f, -2 * tileStep));
		if (!IsOverlapping(otherCollider->GetHitBox(), otherPosition))
		{
			transform->Move(glm::vec2(0.0f, tileStep));
			return;
		}

		transform->Move(glm::vec2(0.0f, 4.0f * tileStep));
		if (!IsOverlapping(otherCollider->GetHitBox(), otherPosition))
		{
			transform->Move(glm::vec2(0.0f, -1.0f * tileStep));
			return;
		}
	}
	else if (static_cast<int>(direction.y) != 0)
	{
		transform->Move(glm::vec2(-2 * tileStep, 0.0f));
		if (!IsOverlapping(otherCollider->GetHitBox(), otherPosition))
		{
			transform->Move(glm::vec2(tileStep, 0.0f));
			return;
		}

		transform->Move(glm::vec2(4.0f * tileStep, 0.0f));
		if (!IsOverlapping(otherCollider->GetHitBox(), otherPosition))
		{
			transform->Move(glm::vec2(-1.0f * tileStep, 0.0f));
			return;
		}
	}

	transform->ResetMovementThisFrame();
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

