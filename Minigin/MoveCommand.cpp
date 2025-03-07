#include "MoveCommand.h"

dae::MoveCommand::MoveCommand(GameObject& controllingObject, glm::vec3 direction) 
	: m_pControllingObject(&controllingObject)
	, m_Direction(direction)
{

}

void dae::MoveCommand::Execute()
{
	auto transform = m_pControllingObject->GetTransform();

	transform->Move(m_Direction);
}
