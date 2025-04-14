#include "Camera.h"

void dae::Camera::Init()
{
}

void dae::Camera::Init(CameraLimits camLimits)
{
	m_CameraLimits = camLimits;
}

void dae::Camera::Update()
{
	if (m_TrackTarget)
	{
		TrackTarget();
		Clamp();
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
	return glm::vec2(m_WindowWidthBase * m_WindowScale, m_WindowHeightBase * m_WindowScale);
}

void dae::Camera::TrackTarget()
{
	auto targetPos = m_pTarget->GetTransform()->GetGlobalPosition();

	m_Position.x = targetPos.x - (m_WindowWidthBase * m_WindowScale) / 2;
	m_Position.y = targetPos.y - (m_WindowHeightBase * m_WindowScale) / 2;


}

void dae::Camera::Clamp()
{	
	glm::vec2 windowSize = GetWindowSize();

	if (m_Position.x < m_CameraLimits.minX)
	{
		m_Position.x = m_CameraLimits.minX;
	}
	if (m_Position.x + windowSize.x > m_CameraLimits.maxX)
	{
		m_Position.x = m_CameraLimits.maxX - windowSize.x;
	}
	if (m_Position.y < m_CameraLimits.minY)
	{
		m_Position.y = m_CameraLimits.minY;
	}
	if (m_Position.y + windowSize.y > m_CameraLimits.maxY)
	{
		m_Position.y = m_CameraLimits.maxY - windowSize.y;
	}
}
