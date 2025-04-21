#pragma once
#include "BaseCollider.h"

namespace bomberman
{
    class OctagonCollider : public BaseCollider
    {
	public:
		OctagonCollider(dae::GameObject& gameObject, CollisionType collisionType = CollisionType::None, float bevelInset = 6);
		~OctagonCollider() override = default;

	private:
		polygon CreateOctagon(float x, float y) const;
		float m_BevelInset;
    };
}


