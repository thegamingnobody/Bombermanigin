#include "ExplosionComponent.h"
#include <SceneManager.h>
#include <Scene.h>
#include <GameObject.h>
#include "BombExplodedEvent.h"
#include "EventManager.h"

bomberman::ExplosionComponent::ExplosionComponent(dae::GameObject& gameObject, int size, float explosionTime)
	: dae::Component(gameObject)
	, m_Size(size)
	, m_ExplosionTime(explosionTime)
{
}

void bomberman::ExplosionComponent::Update(float deltaTime)
{
	auto owner = GetOwner();

	if (m_ExplosionTime > 0.0f)
	{
		m_ExplosionTime -= deltaTime;
	}
	else
	{
		owner->SetShouldBeRemoved();
	}
}
