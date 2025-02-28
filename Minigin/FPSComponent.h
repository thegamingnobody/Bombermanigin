#ifndef FPSCOMPONENT
#define FPSCOMPONENT

#include "Component.h"
#include "TextComponent.h"

namespace dae
{
	class FPSComponent : public Component
	{
	public:
		void Update(float const deltaTime) override;

		int GetFrameCount() const { return m_FrameCount; }
		float GetFPS() const { return m_CurrentFPS; }

		FPSComponent(dae::GameObject& object);
		virtual ~FPSComponent() override = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator= (const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

	private:
		int m_FrameCount;
		float m_CurrentFPS;
		float m_Delay;
		float const m_MaxDelay;
		TextComponent* m_pTextComponent;

	};
}

#endif