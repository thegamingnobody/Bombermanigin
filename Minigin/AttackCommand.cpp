#include "AttackCommand.h"
#include "Event.h"
#include "EventManager.h"

dae::AttackCommand::AttackCommand(GameObject& controllingObject)
	: m_pControllingObject(&controllingObject)
{
}

void dae::AttackCommand::Execute()
{
	glm::vec3 position = m_pControllingObject->GetTransform()->GetGlobalPosition();
	float radius = 50.0f;

	auto arguments = EventArgumentMasks<EventType::BOMB_EXPLODED>::Create(position, radius, m_pControllingObject);

	//Todo: actually spawn a bomb instead of just broadcasting an event
	Event event{ EventType::BOMB_EXPLODED, arguments };

	EventManager::GetInstance().BroadcastEvent(event);
}
