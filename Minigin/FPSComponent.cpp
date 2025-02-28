#include "FPSComponent.h"
#include "GameObject.h"
#include <sstream>
#include <chrono>

void dae::FPSComponent::Update(float const deltaTime)
{
	m_FrameCount++;
	m_Delay += deltaTime;

	if (m_Delay >= m_MaxDelay)
	{
		assert(m_pTextComponent && "No TextComponent found on GameObject");

		std::stringstream stream;
		stream << std::fixed << std::setprecision(1) << m_FrameCount / m_Delay << " FPS";
		m_pTextComponent->SetText(stream.str());

		m_FrameCount = 0;
		m_Delay -= m_MaxDelay;
	}
}

dae::FPSComponent::FPSComponent(dae::GameObject& object)
	: Component(object)
	, m_FrameCount(0)
	, m_CurrentFPS(0.0f)
	, m_Delay(0.0f)
	, m_pTextComponent(nullptr)
	, m_MaxDelay(0.5f)
{
	if (GetOwner()->HasComponent<dae::TextComponent>() and not(m_pTextComponent))
	{
		m_pTextComponent = object.GetComponent<dae::TextComponent>().value();
		m_pTextComponent->SetText("0 FPS");
	}
}
