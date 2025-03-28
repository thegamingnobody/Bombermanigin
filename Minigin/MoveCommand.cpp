#include "MoveCommand.h"
#include "TimeManager.h"

dae::MoveCommand::MoveCommand(GameObject& controllingObject, glm::vec3 direction) 
	: m_pControllingObject(&controllingObject)
	, m_Direction(direction)
{

}

void dae::MoveCommand::Execute()
{
	auto transform = m_pControllingObject->GetTransform();
	//todo: DELTA TIME!!!!!
	auto deltaTime = dae::TimeManager::GetInstance().GetDeltaTime();
	transform->Move(m_Direction * deltaTime);
}
