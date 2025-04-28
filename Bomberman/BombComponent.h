#pragma once
#include "Component.h"

namespace bomberman
{
	class BombComponent : public dae::Component
	{
	public:
		BombComponent(dae::GameObject& gameObject, int radius, float timeToExplode, float timeToDespawn)
			: dae::Component(gameObject)
			, m_Radius(radius)
			, m_TimeToExplode(timeToExplode)
			, m_TimeToDespawn(timeToDespawn)
		{
		}

		void Update(float deltaTime) override;

	private:
		int m_Radius;
		float m_TimeToExplode;
		float m_TimeToDespawn;
		bool m_Exploded{ false };
	};
}

