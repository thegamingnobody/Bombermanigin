#include "HUDManager.h"
#include <GameObject.h>
#include <Camera.h>

void bomberman::HUDManager::Init(dae::TextComponent& textComponent, glm::vec3 screenPos)
{
	m_TextComponent = &textComponent;
	m_ScreenPos = screenPos;

	m_TextComponent->SetText("TestTest");

	m_IsInitialized = true;
}

void bomberman::HUDManager::SetLivesText(int lives)
{
	if (!m_IsInitialized) return;

	m_TextComponent->SetText(m_LeftTextStatic + std::to_string(lives));
}

void bomberman::HUDManager::Update()
{
	if (!m_IsInitialized) return;

	auto& camera = dae::Camera::GetInstance();

 	auto deltaTransformedPosition = camera.TransformPosition(m_ScreenPos).x - m_ScreenPos.x;

	m_TextComponent->GetOwner()->GetTransform()->SetLocalPosition(m_ScreenPos.x - deltaTransformedPosition, m_ScreenPos.y, m_ScreenPos.z);
}