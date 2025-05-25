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
	auto owner = GetOwner();

	BombExplodedEvent event = BombExplodedEvent(owner->GetTransform()->GetGlobalPosition(), 1, owner);
	dae::EventManager::GetInstance().BroadcastEvent(std::move(std::make_unique<BombExplodedEvent>(event)));
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
