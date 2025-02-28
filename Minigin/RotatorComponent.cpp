#include "RotatorComponent.h"

dae::RotatorComponent::RotatorComponent(GameObject* ownerObject, float speed, float radius)
	: PhysicsComponent(ownerObject)
	, m_Speed(speed)
	, m_CurrentAngle(0.0f)
	, m_Radius(radius)
	, m_Center(ownerObject->GetGlobalPosition())
	//, m_CurrentOffset(0.0f, 0.0f)
{
}

void dae::RotatorComponent::Update(float const deltaTime)
{
	auto owner = GetOwner();
	auto parentOfOwner = owner->GetParentObject();

	if (parentOfOwner != nullptr)
	{
		m_Center = parentOfOwner->GetGlobalPosition();
	}

	float rotationSpeed = (m_Speed / 360.0f);
	m_CurrentAngle += rotationSpeed * deltaTime;

	glm::vec2 currentOffset = { (cosf(m_CurrentAngle) * m_Radius), -(sinf(m_CurrentAngle) * m_Radius) };
	
	owner->SetLocalPosition(m_Center.x + currentOffset.x, m_Center.y + currentOffset.y);
}