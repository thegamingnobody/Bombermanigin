#pragma once
#include "Component.h"

namespace bomberman
{
	class BombComponent : public dae::Component
	{
	public:
		BombComponent(dae::GameObject& gameObject, int explosionSize, float timeToExplode, float explosionTime)
			: dae::Component(gameObject)
			, m_ExplosionSize(explosionSize)
			, m_TimeToExplode(timeToExplode)
			, m_ExplosionTime(explosionTime)
		{
		}

		void Update(float deltaTime) override;

	private:

		void SpawnExplosion(int size);

		int m_ExplosionSize;
		float m_TimeToExplode;
		float m_ExplosionTime;
		bool m_Exploded{ false };
	};
}

