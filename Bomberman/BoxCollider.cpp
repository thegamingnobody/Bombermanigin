#include "BoxCollider.h"
#include <GameObject.h>
#include <Renderer.h>
#include "CollidersManager.h"

#include <iostream>

bomberman::BoxCollider::BoxCollider(dae::GameObject& gameObject, CollisionType collisionType, Box hitBox)
	: dae::Component(gameObject)
	, m_HitBox(hitBox)
	, m_CollisionType(collisionType)
{
	CollidersManager::GetInstance().AddCollider(*this);
}

void bomberman::BoxCollider::Update(float const /*deltaTime*/)
{
	auto& collidersManager = CollidersManager::GetInstance();
	auto colliders = collidersManager.GetColliders();

	for (const auto& collider : colliders)
	{
		if (collider == this) continue;

		if (!collider->ShouldCheckCollision()) continue;

		Box other = collider->GetHitBox();
		auto otherPosition = collider->GetOwner()->GetTransform()->GetGlobalPosition();
		other.x += otherPosition.x;
		other.y += otherPosition.y;

		if (IsOverlapping(other))
		{
			std::cout << "Collision\n";
			// Handle collision
			// For example, you can call a method on the other collider or trigger an event
			// collider->OnCollision(this);
		}
	}
}

void bomberman::BoxCollider::Render() const
{
	glm::vec3 position = GetOwner()->GetTransform()->GetGlobalPosition();
	glm::vec2 size = { m_HitBox.width, m_HitBox.height };
	//glm::vec3 color = { 1.0f, 0.0f, 0.0f }; // Red color
	auto& renderer = dae::Renderer::GetInstance();

	renderer.RenderRect(position.x + m_HitBox.x, position.y + m_HitBox.y, size.x, size.y);

}

bool bomberman::BoxCollider::ShouldCheckCollision() const
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

bool bomberman::BoxCollider::IsOverlapping(Box other) const
{
	auto position = GetOwner()->GetTransform()->GetGlobalPosition();

	if (position.x + m_HitBox.x > other.x + other.width or other.x > position.x + m_HitBox.x + m_HitBox.width) return false;
	if (position.y + m_HitBox.y > other.y + other.height or other.y > position.y + m_HitBox.y + m_HitBox.height) return false;

	return true;
}
