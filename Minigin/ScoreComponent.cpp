#include "ScoreComponent.h"
#include "EventManager.h"

dae::ScoreComponent::ScoreComponent(GameObject& ownerObject)
	: Component(ownerObject)
	, m_Score(0)
{
}

void dae::ScoreComponent::AddScore(int addedScore)
{
	if (addedScore < 0) return;

	m_Score += addedScore;

	auto owner = GetOwner();
	assert(owner != nullptr);

	Event event{ EventType::SCORE_ADDED, EventArgumentMasks<EventType::SCORE_ADDED>::Create(owner, m_Score) };
	EventManager::GetInstance().BroadcastEvent(event);
}

void dae::ScoreComponent::RemoveScore(int removedScore)
{
	if (removedScore < 0) return;

	m_Score -= removedScore;
}

void dae::ScoreComponent::Notify(const Event& event)
{
	switch (event.m_EventType)
	{
	case EventType::OBJECT_DAMAGED:
		auto [damagedObject, newHealth] = event.GetArgumentsAsTuple<EventType::OBJECT_DAMAGED>();

		auto owner = GetOwner();
		assert(owner != nullptr);

		if (owner != damagedObject)
		{
			AddScore(100);
		}
		break;
	}
}
