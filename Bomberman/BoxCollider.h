#pragma once
#include "BaseCollider.h"
#include <GameObject.h>
#include "Grid.h"

namespace bomberman
{
	struct Box
	{
		float x{ 0.0f };
		float y{ 0.0f };
		float width{ 0.0f };
		float height{ 0.0f };

		Box(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f)
			: x(x), y(y), width(width), height(height) 
		{
		}
	};

    class BoxCollider : public BaseCollider
    {
	public:
		BoxCollider(dae::GameObject& gameObject, CollisionType collisionType = CollisionType::None, Box hitBox = Box(0.0f, 0.0f, TILE_SIZE, TILE_SIZE));
		~BoxCollider() override = default;

	private:
		polygon CreateVerts(Box hitbox) const;
	};
}


