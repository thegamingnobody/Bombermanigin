#pragma once
#include <Component.h>

namespace bomberman
{
    class ExplosionComponent : public dae::Component
    {
	public:
		ExplosionComponent(dae::GameObject& gameObject, int size, float explosionTime)
			: dae::Component(gameObject)
			, m_Size(size)
			, m_ExplosionTime(explosionTime)
		{
		}

		void Update(float deltaTime) override;

	private:
		int m_Size;
		float m_ExplosionTime;
    };
}

