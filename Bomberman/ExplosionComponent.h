#pragma once
#include <Component.h>

namespace bomberman
{
    class ExplosionComponent : public dae::Component
    {
	public:
		ExplosionComponent(dae::GameObject& gameObject, int size, float explosionTime);

		void Update(float deltaTime) override;

	private:
		int m_Size;
		float m_ExplosionTime;
    };
}

