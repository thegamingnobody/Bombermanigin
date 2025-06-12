#include "HUDManager.h"
#include <GameObject.h>
#include <Camera.h>
#include <sstream>

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

	m_Lives = lives;

	SetText();
}

void bomberman::HUDManager::SetScoreText(int score)
{
	if (!m_IsInitialized) return;

	m_Score = score;

	SetText();
}

void bomberman::HUDManager::Update()
{
	if (!m_IsInitialized) return;

	auto& camera = dae::Camera::GetInstance();

 	auto deltaTransformedPosition = camera.TransformPosition(m_ScreenPos).x - m_ScreenPos.x;

	m_TextComponent->GetOwner()->GetTransform()->SetLocalPosition(m_ScreenPos.x - deltaTransformedPosition, m_ScreenPos.y, m_ScreenPos.z);
}

void bomberman::HUDManager::SetText()
{
	std::stringstream ss;

	ss << m_ScoreTextStatic << m_Score << "      " << m_LeftTextStatic << m_Lives;
	m_TextComponent->SetText(ss.str());
}
