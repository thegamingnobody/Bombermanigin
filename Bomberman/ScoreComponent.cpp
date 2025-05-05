#include "ScoreComponent.h"
#include "EventManager.h"
#include <cassert>
#include "EventTypes.h"
#include "ObjectDamagedEvent.h"
#include "ScoreAddedEvent.h"

bomberman::ScoreComponent::ScoreComponent(dae::GameObject& ownerObject)
	: Component(ownerObject)
	, m_Score(0)
{
}

void bomberman::ScoreComponent::AddScore(int addedScore)
{
	if (addedScore < 0) return;

	m_Score += addedScore;

	auto owner = GetOwner();
	assert(owner != nullptr);

	ScoreAddedEvent event{ owner, m_Score };
	dae::EventManager::GetInstance().BroadcastEvent(event);
}

void bomberman::ScoreComponent::RemoveScore(int removedScore)
{
	if (removedScore < 0) return;

	m_Score -= removedScore;
}

void bomberman::ScoreComponent::Notify(const dae::Event&)
{
	//Todo: fix?
}
