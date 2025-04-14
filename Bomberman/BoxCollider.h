#pragma once
#include <Component.h>

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

	enum class CollisionType
	{
		None,
		Wall,
		Entity
	};

    class BoxCollider : public dae::Component
    {
	public:
		BoxCollider(dae::GameObject& gameObject, CollisionType collisionType = CollisionType::None, Box hitBox = Box());
		~BoxCollider() override = default;

		void Update(float const deltaTime) override;
		void Render() const override;
	
		Box GetHitBox() const { return m_HitBox; }

		bool IsOverlapping(Box other) const;
	private:
		//Checks if this object should check for collisions
		bool ShouldCheckCollision() const;

		Box m_HitBox;
		CollisionType m_CollisionType;
	};
}


