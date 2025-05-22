#include "ExplosionComponent.h"
#include <SceneManager.h>
#include <Scene.h>
#include <GameObject.h>
#include "BombExplodedEvent.h"
#include "EventManager.h"

void bomberman::ExplosionComponent::Update(float deltaTime)
{
	if (m_ExplosionTime > 0.0f)
	{
		m_ExplosionTime -= deltaTime;
	}
	else
	{
		auto owner = GetOwner();

		BombExplodedEvent event = BombExplodedEvent(owner->GetTransform()->GetGlobalPosition(), 1, owner);
		dae::EventManager::GetInstance().BroadcastEvent(std::move(std::make_unique<BombExplodedEvent>(event)));

		GetOwner()->SetShouldBeRemoved();
	}
}
