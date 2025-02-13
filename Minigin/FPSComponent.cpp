#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <sstream>
#include <chrono>

void dae::FPSComponent::Update(float const deltaTime)
{
	m_FrameCount++;
	m_Delay += deltaTime;

	if (m_Delay >= m_MaxDelay)
	{
		auto textComponent{ GetOwner()->GetComponent<dae::TextComponent>() };
		if (textComponent.has_value())
		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(1) << m_FrameCount / m_Delay << " FPS";
			textComponent.value()->SetText(stream.str());
		}
		else
		{
			std::cout << "[FPS COMPONENT] No TextComponent found on GameObject\n";
		}

		m_FrameCount = 0;
		m_Delay = 0;
	}
}

dae::FPSComponent::FPSComponent(dae::GameObject* object)
	: Component(object)
	, m_FrameCount(0)
	, m_CurrentFPS(0.0f)
	, m_Delay(0.0f)
{
}
