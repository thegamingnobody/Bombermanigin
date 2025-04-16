#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(GameObject& ownerObject, float x, float y, float z)
	: Component(ownerObject)
	, m_LocalPosition(x, y, z)
	, m_GlobalPosition(x, y, z)
	, m_MovementThisFrame(0.0f, 0.0f, 0.0f)
{
}
dae::Transform::Transform(GameObject& ownerObject, glm::vec3 position)
	: Component(ownerObject)
	, m_LocalPosition(position)
	, m_GlobalPosition(position)
	, m_MovementThisFrame(0.0f, 0.0f, 0.0f)
{
}
dae::Transform::Transform(GameObject& ownerObject, glm::vec2 position)
	: Component(ownerObject)
	, m_LocalPosition(position.x, position.y, 0.0f)
	, m_GlobalPosition(position.x, position.y, 0.0f)
	, m_MovementThisFrame(0.0f, 0.0f, 0.0f)
{
}

const glm::vec3& dae::Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}
const glm::vec3& dae::Transform::GetGlobalPosition()
{
	if (m_ShouldUpdatePosition)
	{
		UpdateGlobalPosition();
	}
	return m_GlobalPosition;
}

glm::vec3 dae::Transform::GetPredictedPosition()
{
	glm::vec3 tempGLobalPosition = CalculateGlobalPosition();
	glm::vec3 predictedPosition = tempGLobalPosition + m_MovementThisFrame;
	return predictedPosition;
}

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	m_ShouldUpdatePosition = true;
}
void dae::Transform::SetLocalPosition(glm::vec3 pos)
{
	SetLocalPosition(pos.x, pos.y, pos.z);
}
void dae::Transform::SetLocalPosition(glm::vec2 pos)
{
	SetLocalPosition(pos.x, pos.y, 0.0f);
}

void dae::Transform::SetDirty()
{
	m_ShouldUpdatePosition = true;
}

void dae::Transform::UpdateGlobalPosition()
{
	auto owner = GetOwner();

	auto newGlobalPosition = CalculateGlobalPosition();
	m_GlobalPosition = newGlobalPosition;
	owner->SetChildTransformsDirty();
	m_ShouldUpdatePosition = false;
}

void dae::Transform::Move(float x, float y, float z)
{
	m_MovementThisFrame.x += x;
	m_MovementThisFrame.y += y;
	m_MovementThisFrame.z += z;
	m_ShouldUpdatePosition = true;
}
void dae::Transform::Move(glm::vec3 pos)
{
	Move(pos.x, pos.y, pos.z);
}
void dae::Transform::Move(glm::vec2 pos)
{
	Move(pos.x, pos.y, 0.0f);
}

void dae::Transform::LateUpdate()
{
	m_LocalPosition += m_MovementThisFrame;
	m_MovementThisFrame = glm::vec3(0.0f, 0.0f, 0.0f);
}

void dae::Transform::ResetMovementThisFrame()
{
	m_MovementThisFrame = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 dae::Transform::CalculateGlobalPosition() const
{
	glm::vec3 result{};
	auto owner = GetOwner();

	auto parentOfOwner{ owner->GetParentObject() };

	glm::vec3 parentOfOwnerPosition{ 0.0f, 0.0f, 0.0f };

	if (parentOfOwner != nullptr)
	{
		parentOfOwnerPosition = parentOfOwner->GetTransform()->GetGlobalPosition();
	}

	result = parentOfOwnerPosition + m_LocalPosition;
	return result;
}
