#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(GameObject& ownerObject, float x, float y, float z)
	: Component(ownerObject)
	, m_LocalPosition(x, y, z)
	, m_ParentPosition(0.0f, 0.0f, 0.0f)
	, m_GlobalPosition(x, y, z)
{
}
dae::Transform::Transform(GameObject& ownerObject, glm::vec3 position)
	: Component(ownerObject)
	, m_LocalPosition(position)
	, m_ParentPosition(0.0f, 0.0f, 0.0f)
	, m_GlobalPosition(position)
{
}
dae::Transform::Transform(GameObject& ownerObject, glm::vec2 position)
	: Component(ownerObject)
	, m_LocalPosition(position.x, position.y, 0.0f)
	, m_ParentPosition(0.0f, 0.0f, 0.0f)
	, m_GlobalPosition(position.x, position.y, 0.0f)
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

void dae::Transform::SetParentPosition(const float x, const float y, const float z)
{
	m_ParentPosition.x = x;
	m_ParentPosition.y = y;
	m_ParentPosition.z = z;
	m_ShouldUpdatePosition = true;
}
void dae::Transform::SetParentPosition(glm::vec3 pos)
{
	SetParentPosition(pos.x, pos.y, pos.z);
}
void dae::Transform::SetParentPosition(glm::vec2 pos)
{
	SetParentPosition(pos.x, pos.y, 0.0f);
}

void dae::Transform::SetDirty()
{
	m_ShouldUpdatePosition = true;
}

void dae::Transform::UpdateGlobalPosition()
{
	auto owner = GetOwner();

	if (owner != nullptr)
	{
		auto parentOfOwner{ owner->GetParentObject() };
		if (parentOfOwner != nullptr)
		{
			SetParentPosition(parentOfOwner->GetTransform()->GetGlobalPosition());
		}
	}

	m_GlobalPosition = m_ParentPosition + m_LocalPosition;
	m_ShouldUpdatePosition = false;
}

void dae::Transform::Move(float x, float y, float z)
{
	m_LocalPosition.x += x;
	m_LocalPosition.y += y;
	m_LocalPosition.z += z;
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
