#include "AttackCommand.h"
#include "Event.h"
#include "EventManager.h"

bomberman::AttackCommand::AttackCommand(dae::GameObject& controllingObject)
	: m_pControllingObject(&controllingObject)
{
}

void bomberman::AttackCommand::Execute()
{
	glm::vec3 position = m_pControllingObject->GetTransform()->GetGlobalPosition();
	float radius = 50.0f;

	auto arguments = dae::EventArgumentMasks<dae::EventType::BOMB_EXPLODED>::Create(position, radius, m_pControllingObject);

	//Todo: actually spawn a bomb instead of just broadcasting an event
	dae::Event event{ dae::EventType::BOMB_EXPLODED, arguments };

	dae::EventManager::GetInstance().BroadcastEvent(event);
}
