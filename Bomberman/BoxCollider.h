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

    class BoxCollider : public dae::Component
    {
	public:
		BoxCollider(dae::GameObject& gameObject, Box hitBox = Box());
		~BoxCollider() override = default;

		//void Update() override;
		void Render() const override;
	
	private:
		Box m_HitBox{};		
	};
}


