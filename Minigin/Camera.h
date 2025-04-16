#pragma once
#include "Singleton.h"
#include <glm.hpp>
#include "GameObject.h"

namespace dae
{
	struct CameraLimits
	{
		float minX{ 0.0f };
		float maxX{ 0.0f };
		float minY{ 0.0f };
		float maxY{ 0.0f };

		CameraLimits(float minX = 0.0f, float maxX = 0.0f, float minY = 0.0f, float maxY = 0.0f)
			: minX(minX), maxX(maxX), minY(minY), maxY(maxY) 
		{
		}
	};

	class Camera final : public Singleton<Camera>
	{
	public:
		void Init();
		void Init(CameraLimits camLimits);
		void Update();

		void SetCameraLimits(CameraLimits camLimits) { m_CameraLimits = camLimits; }

		void SetTrackingTarget(dae::GameObject& trackingTarget);
		void EnableTracking(bool enable) { m_TrackTarget = enable; }

		glm::vec3 TransformPosition(const glm::vec3& position) const;
		bool IsInView(const glm::vec3& position) const;

		glm::vec2 GetWindowSize() const;
		float GetWindowScale() const { return m_WindowScale; }
	private:
		void TrackTarget();
		void Clamp();

		glm::vec2 m_Position{ 0, -96 };
		
		int const m_WindowWidthBase{ 256 };
		int const m_WindowHeightBase{ 240 };
		//makes the window bigger but doesn't scale any of the sprites
		float const m_WindowScale{ 3.0f };

		CameraLimits m_CameraLimits{};

		GameObject* m_pTarget{ nullptr };
		bool m_TrackTarget{ false };

	};
}

