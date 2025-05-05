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

	float lineWidth = crossSize * TILE_SIZE * 0.8f;
	float lineHeight = TILE_SIZE / 2;
	////float lineX = ((crossSize - 1) / 2) * TILE_SIZE * m_Modifier;
	float lineX = crossSize * 0.1f * TILE_SIZE;
	float lineY = (TILE_SIZE * crossSize * 0.41f);

	Box horizontalLine{ lineX, lineY, lineWidth, lineHeight};
	Box verticalLine{ lineY, lineX, lineHeight, lineWidth };

	//Box horizontalLine{ lineX, 0.0f, lineWidth, lineHeight };
	//Box verticalLine{ lineY, lineX, lineHeight, lineWidth };

	//Box horizontalLine{ -((crossSize - 1) / 2) * TILE_SIZE, 0.0f, crossSize * TILE_SIZE, TILE_SIZE};
	//Box verticalLine{ 0.0f, -((crossSize - 1) / 2) * TILE_SIZE, TILE_SIZE, crossSize * TILE_SIZE };

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
		{cross.horizontalX, cross.horizontalY},
		{cross.horizontalX + a, cross.horizontalY},
		{cross.horizontalX + a, cross.horizontalY - c},
		{cross.horizontalX + a + cross.verticalWidth, cross.horizontalY - c},
		{cross.horizontalX + a + cross.verticalWidth, cross.horizontalY},
		{cross.horizontalX + cross.horizontalWidth, cross.horizontalY},
		{cross.horizontalX + cross.horizontalWidth, cross.horizontalY + cross.horizontalHeight},
		{cross.horizontalX + a + cross.verticalWidth, cross.horizontalY + cross.horizontalHeight},
		{cross.horizontalX + a + cross.verticalWidth, cross.verticalY + cross.verticalHeight},
		{cross.horizontalX + a, cross.verticalY + cross.verticalHeight},
		{cross.horizontalX + a,  cross.horizontalY + cross.horizontalHeight},
		{cross.horizontalX,  cross.horizontalY + cross.horizontalHeight},
	};														 
	//return 
	//{
	//	{a,			c},
	//	{a,			0.0f},
	//	{a + cross.verticalWidth,		0.0f},
	//	{a + cross.verticalWidth,		c},
	//	{a + cross.verticalWidth + b,		c},
	//	{a + cross.verticalWidth + b,		c + cross.horizontalHeight},
	//	{a + cross.verticalWidth,		c + cross.horizontalHeight},
	//	{a + cross.verticalWidth,		c + cross.horizontalHeight + d},
	//	{a,		c + cross.horizontalHeight + d},
	//	{a,		c + cross.horizontalHeight},
	//	{0.0f,		c + cross.horizontalHeight},
	//	{0.0f,		c},
	//};														 
}
