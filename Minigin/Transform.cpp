#include "Transform.h"

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	m_PositionDirty = true;
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
	m_PositionDirty = true;
}
void dae::Transform::SetParentPosition(glm::vec3 pos)
{
	SetParentPosition(pos.x, pos.y, pos.z);
}
void dae::Transform::SetParentPosition(glm::vec2 pos)
{
	SetParentPosition(pos.x, pos.y, 0.0f);
}

void dae::Transform::UpdateGlobalPosition()
{
	//todo: fix this
	m_GlobalPosition = m_ParentPosition + m_LocalPosition;
	m_PositionDirty = false;
}

void dae::Transform::Update(float const /*deltaTime*/)
{
	if (m_PositionDirty)
	{
		UpdateGlobalPosition();
	}
}

dae::Transform::Transform(GameObject* ownerObject, float x, float y, float z)
	: Component(ownerObject)
	, m_LocalPosition(x, y, z)
	, m_ParentPosition(0.0f, 0.0f, 0.0f)
	, m_GlobalPosition(x, y, z)
{
}
dae::Transform::Transform(GameObject* ownerObject, glm::vec3 position)
	: Component(ownerObject)
	, m_LocalPosition(position)
	, m_ParentPosition(0.0f, 0.0f, 0.0f)
	, m_GlobalPosition(position)
{
}
dae::Transform::Transform(GameObject* ownerObject, glm::vec2 position)
	: Component(ownerObject)
	, m_LocalPosition(position.x, position.y, 0.0f)
	, m_ParentPosition(0.0f, 0.0f, 0.0f)
	, m_GlobalPosition(position.x, position.y, 0.0f)
{
}