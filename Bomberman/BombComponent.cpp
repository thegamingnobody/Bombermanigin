#include "BombComponent.h"
#include "GameObject.h"
#include "CrossCollider.h"

void bomberman::BombComponent::Update(float deltaTime)
{
	if (m_Exploded)
	{
		m_TimeToDespawn -= deltaTime;
	}
	else
	{
		m_TimeToExplode -= deltaTime;
	}

	//Todo: animate bomb (based on time left?)
	if (m_TimeToExplode <= 0.0f)
	{
		if (m_Exploded) return;

		m_Exploded = true;

		//Todo: Trigger explosion
		GetOwner()->AddComponent<bomberman::CrossCollider>(*GetOwner(), bomberman::CollisionType::Bomb);
	}
}