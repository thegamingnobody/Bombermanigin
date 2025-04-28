#pragma once
#include "Component.h"

namespace bomberman
{
	class BombComponent : public dae::Component
	{
	public:
		BombComponent(dae::GameObject& gameObject, int radius, float timeToExplode)
			: dae::Component(gameObject)
			, m_Radius(radius)
			, m_TimeToExplode(timeToExplode)
		{
		}

		void Update(float deltaTime) override
		{
			m_TimeToExplode -= deltaTime;
			if (m_TimeToExplode <= 0.0f)
			{
				//Todo: Trigger explosion
			}
		}

	private:
		int m_Radius;
		float m_TimeToExplode;
	};
}

