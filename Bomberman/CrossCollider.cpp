#include "CrossCollider.h"
#include "CollidersManager.h"
#include "Grid.h"
#include "..\3rdParty\Imgui\imgui.h"

bomberman::CrossCollider::CrossCollider(dae::GameObject& gameObject, CollisionType collisionType)
	: BaseCollider(gameObject, collisionType)
{
	CollidersManager::GetInstance().AddCollider(*this);
	int crossSize = 3;

	//float m_Modifier = 0.75f;
	float collisionInset = 0.0f;

	float lineWidth = crossSize * TILE_SIZE * 0.8f;
	float lineHeight = TILE_SIZE / 2;
	float lineX = crossSize * 0.1f * TILE_SIZE;
	float lineY = (TILE_SIZE * crossSize * 0.41f);

	Box horizontalLine	{ lineX + collisionInset, lineY + collisionInset, lineWidth  - (2 * collisionInset),	lineHeight - (2 * collisionInset)};
	Box verticalLine	{ lineY + collisionInset, lineX + collisionInset, lineHeight - (2 * collisionInset),	lineWidth  - (2 * collisionInset)};

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
	//float b = cross.horizontalWidth - a - cross.verticalWidth;
	float c = cross.horizontalY - cross.verticalY;
	//float d = cross.verticalHeight - c - cross.horizontalHeight;

	return 
	{
		{cross.horizontalX,  cross.horizontalY + cross.horizontalHeight},								// A
		{cross.horizontalX, cross.horizontalY},															// B
		{cross.horizontalX + cross.horizontalWidth, cross.horizontalY},									// 4
		{cross.horizontalX + cross.horizontalWidth, cross.horizontalY + cross.horizontalHeight},		// 5
		{cross.horizontalX + a,  cross.horizontalY + cross.horizontalHeight},							// 9
		{cross.horizontalX + a, cross.horizontalY - c},													// 1
		{cross.horizontalX + a + cross.verticalWidth, cross.horizontalY - c},							// 2
		{cross.horizontalX + a + cross.verticalWidth, cross.verticalY + cross.verticalHeight},			// 7
		{cross.horizontalX + a, cross.verticalY + cross.verticalHeight},								// 8
		{cross.horizontalX + a, cross.horizontalY},														// 0
		{cross.horizontalX + a + cross.verticalWidth, cross.horizontalY},								// 3
		{cross.horizontalX + a + cross.verticalWidth, cross.horizontalY + cross.horizontalHeight},		// 6
	};														 
}
