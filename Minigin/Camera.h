#pragma once
#include "Singleton.h"
#include <glm.hpp>
#include "Transform.h"
#include "GameObject.h"

namespace dae
{
	class Camera final : public Singleton<Camera>
	{
	public:
		void Init();
		void Update();

		void SetTrackingTarget(dae::GameObject& trackingTarget);
		void EnableTracking(bool enable) { m_TrackTarget = enable; }

		glm::vec3 TransformPosition(const glm::vec3& position) const;
		bool IsInView(const glm::vec3& position) const;

		//glm::vec3 ClampPosition(const glm::vec3& position) const;

		glm::vec2 GetWindowSize() const;
	private:
		void TrackTarget();

		glm::vec2 m_Position{ 0,-96 };

		GameObject* m_pTarget{ nullptr };
		bool m_TrackTarget{ false };

		int const m_windowWidthBase{ 256 };
		int const m_windowHeightBase{ 240 };
		//makes the window bigger but doesn't scale any of the sprites
		float const m_windowScale{ 3.0f };

		//glm::vec2 m_Scale{ 1,1 };
		//glm::vec2 m_ViewportSize{ 800,600 };
	};
}

