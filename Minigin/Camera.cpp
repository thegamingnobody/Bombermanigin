#include "Camera.h"

void dae::Camera::Init()
{
}

void dae::Camera::Update()
{
	if (m_TrackTarget)
	{
		TrackTarget();
	}
}

void dae::Camera::SetTrackingTarget(dae::GameObject& trackingTarget)
{
	m_pTarget = &trackingTarget;
	m_TrackTarget = true;
}

glm::vec3 dae::Camera::TransformPosition(const glm::vec3& position) const
{
	glm::vec3 result{};

	result.x = position.x - m_Position.x;
	result.y = position.y - m_Position.y;
	result.z = position.z;

    return result;
}

bool dae::Camera::IsInView(const glm::vec3& position) const
{
	glm::vec2 windowSize = GetWindowSize();

	if (position.x < m_Position.x) return false;
	if (position.x > m_Position.x + windowSize.x) return false;
	if (position.y < m_Position.y) return false;
	if (position.y > m_Position.y + windowSize.y) return false;

	return true;
}

glm::vec2 dae::Camera::GetWindowSize() const
{
	return glm::vec2(m_windowWidthBase * m_windowScale, m_windowHeightBase * m_windowScale);
}

void dae::Camera::TrackTarget()
{
	auto targetPos = m_pTarget->GetTransform()->GetGlobalPosition();

	m_Position.x = targetPos.x - (m_windowWidthBase * m_windowScale) / 2;
	m_Position.y = targetPos.y - (m_windowHeightBase * m_windowScale) / 2;
}
