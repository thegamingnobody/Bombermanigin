#include "ScoreComponent.h"
#include "EventManager.h"
#include <cassert>

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

	//dae::Event event{dae:: EventType::SCORE_ADDED, dae::EventArgumentMasks<dae::EventType::SCORE_ADDED>::Create(owner, m_Score) };
	//dae::EventManager::GetInstance().BroadcastEvent(event);
}

void bomberman::ScoreComponent::RemoveScore(int removedScore)
{
	if (removedScore < 0) return;

	m_Score -= removedScore;
}

void bomberman::ScoreComponent::Notify(const dae::Event& event)
{
	event;
	//switch (event.m_EventType)
	//{
	//case dae::EventType::OBJECT_DAMAGED:
	//	auto [damagedObject, newHealth] = event.GetArgumentsAsTuple<dae::EventType::OBJECT_DAMAGED>();

	//	auto owner = GetOwner();
	//	assert(owner != nullptr);

	//	if (owner != damagedObject)
	//	{
	//		AddScore(100);
	//	}
	//	break;
	//}
}
