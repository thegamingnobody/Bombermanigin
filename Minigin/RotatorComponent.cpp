#include "RotatorComponent.h"

dae::RotatorComponent::RotatorComponent(GameObject* ownerObject, float speed, float radius)
	: PhysicsComponent(ownerObject)
	, m_Speed(speed)
	, m_CurrentAngle(0.0f)
	, m_Radius(radius)
	, m_Center(ownerObject->GetPosition().GetPosition())
	, m_CurrentOffset(0.0f, 0.0f)
{
}

void dae::RotatorComponent::FixedUpdate(float const fixedTimeStep)
{
	auto owner = GetOwner();
	glm::vec2 currentOwnerObjectCenter = { owner->GetPosition().GetPosition().x - m_CurrentOffset.x, owner->GetPosition().GetPosition().y - m_CurrentOffset.y };

	if (m_Center != currentOwnerObjectCenter)
	{
		m_Center = currentOwnerObjectCenter;
	}

	float rotationSpeed = (m_Speed / 360.0f);

	m_CurrentAngle += rotationSpeed * fixedTimeStep;
	
	m_CurrentOffset = { (cosf(m_CurrentAngle) * m_Radius), -(sinf(m_CurrentAngle) * m_Radius) };
	owner->SetPosition(m_Center.x + m_CurrentOffset.x, m_Center.y + m_CurrentOffset.y);
}

void dae::RotatorComponent::Update(float const){}
