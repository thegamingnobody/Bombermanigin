#include "RotatorComponent.h"

dae::RotatorComponent::RotatorComponent(GameObject& ownerObject, float speed, float radius)
	: PhysicsComponent(ownerObject)
	, m_Speed(speed)
	, m_CurrentAngle(0.0f)
	, m_Radius(radius)
{
}

void dae::RotatorComponent::Update(float const deltaTime)
{
	auto owner = GetOwner();

	float rotationSpeed = (m_Speed / 360.0f);
	m_CurrentAngle += rotationSpeed * deltaTime;

	if (m_CurrentAngle >= 360.f)
	{
		m_CurrentAngle -= 360.0f;
	}
	else if (m_CurrentAngle <= -360.0f)
	{
		m_CurrentAngle += 360.0f;
	}

	glm::vec2 currentOffset = { (cosf(m_CurrentAngle) * m_Radius), -(sinf(m_CurrentAngle) * m_Radius) };
	
	owner->GetTransform()->SetLocalPosition(currentOffset.x, currentOffset.y);
}