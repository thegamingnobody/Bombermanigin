#include "MoveCommand.h"
#include "TimeManager.h"
#include "ServiceLocator.h"
#include "DAE_SDL_Soundsystem.h"
#include "ServiceLocator.h"

bomberman::MoveCommand::MoveCommand(dae::GameObject& controllingObject, glm::vec3 direction)
	: m_pControllingObject(&controllingObject)
	, m_Direction(direction)
{

}

void bomberman::MoveCommand::Execute()
{
	auto transform = m_pControllingObject->GetTransform();
	//todo: DELTA TIME!!!!!
	auto deltaTime = dae::TimeManager::GetInstance().GetDeltaTime();
	transform->Move(m_Direction * deltaTime);

	dae::ServiceLocator::GetSoundSystem().PlaySound(0, 0.5f);

}
