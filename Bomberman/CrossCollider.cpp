#include "CrossCollider.h"
#include "CollidersManager.h"
#include "Grid.h"

bomberman::CrossCollider::CrossCollider(dae::GameObject& gameObject, CollisionType collisionType)
	: BaseCollider(gameObject, collisionType)
{
	CollidersManager::GetInstance().AddCollider(*this);
	int crossSize = 3;

	Box horizontalLine{ -((crossSize - 1) / 2) * TILE_SIZE, 0.0f, crossSize * TILE_SIZE, TILE_SIZE };
	Box verticalLine{ 0.0f, -((crossSize - 1) / 2) * TILE_SIZE, TILE_SIZE, crossSize * TILE_SIZE };
	m_Polygon = CreateCross(Cross(horizontalLine, verticalLine));
	m_Axes = CalculateAxes(m_Polygon);
}

polygon bomberman::CrossCollider::CreateCross(Cross cross) const
{
	//
	//		  1*2
	//		c ***
	//		  ***
	//		  ***
	//		  ***
	//	  a	  ***   b
	//	B*****O*3*****4
	//	***************
	//	A*****9*6*****5
	//	  	  ***   
	//		  ***
	//		  ***
	//		  ***
	//		d ***
	//		  8*7

	float a = cross.verticalX - cross.horizontalX;
	float b = cross.horizontalWidth - a - cross.verticalWidth;
	float c = cross.horizontalY - cross.verticalY;
	float d = cross.verticalHeight - c - cross.horizontalHeight;

	return 
	{
		{0.0f,						0.0f},
		{0.0f,						-c},
		{cross.verticalWidth,		-c},
		{cross.verticalWidth,		0.0f},
		{cross.verticalWidth + b,	0.0f},
		{cross.verticalWidth + b,	cross.horizontalHeight},
		{cross.verticalWidth,		cross.horizontalHeight},
		{cross.verticalWidth,		cross.horizontalHeight + d},
		{0.0f,						cross.horizontalHeight + d},
		{0.0f,						cross.horizontalHeight},
		{-a,						cross.horizontalHeight},
		{-a,						0.0f}
	};														 
}
