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
		if (m_pTextComponent)
		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(1) << m_FrameCount / m_Delay << " FPS";
			m_pTextComponent->SetText(stream.str());
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
	m_pTextComponent = object->GetComponent<dae::TextComponent>().value();
}
