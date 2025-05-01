#pragma once
#include "BaseCollider.h"
#include "BoxCollider.h"

namespace bomberman
{
    struct Cross
    {
        float horizontalX{};
        float horizontalY{};

		float horizontalWidth{};
        float horizontalHeight{};

        float verticalX{};
        float verticalY{};

		float verticalWidth{};
		float verticalHeight{};

		Cross(float horizontalX, float horizontalY, float horizontalWidth, float horizontalHeight,
			float verticalX, float verticalY, float verticalWidth, float verticalHeight)
			: horizontalX(horizontalX)
			, horizontalY(horizontalY)
			, horizontalWidth(horizontalWidth)
			, horizontalHeight(horizontalHeight)
			, verticalX(verticalX)
			, verticalY(verticalY)
			, verticalWidth(verticalWidth)
			, verticalHeight(verticalHeight)
		{
		}

        Cross(Box horizontalLine, Box verticalLine)
			: horizontalX(horizontalLine.x)
			, horizontalY(horizontalLine.y)
			, horizontalWidth(horizontalLine.width)
			, horizontalHeight(horizontalLine.height)
			, verticalX(verticalLine.x)
			, verticalY(verticalLine.y)
			, verticalWidth(verticalLine.width)
			, verticalHeight(verticalLine.height)
		{
		}
    };

    class CrossCollider : public BaseCollider
    {
    public:
        CrossCollider(dae::GameObject& gameObject, CollisionType collisionType = CollisionType::None);
        ~CrossCollider() override = default;

		void RenderImGui() override;
    private:
        //the position is the topleft corner of the horizontal line
        polygon CreateCross(Cross cross) const;
		float m_Modifier{};
    };
}

