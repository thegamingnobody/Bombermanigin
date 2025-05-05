#include "MoveCommand.h"
#include "TimeManager.h"
#include "ServiceLocator.h"
#include "DAE_SDL_Soundsystem.h"
#include "ServiceLocator.h"
#include "SoundIds.h"

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

	if (m_Direction.x != 0.0f)
	{
		dae::ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(bomberman::SoundId::WalkHorizontal), 1.0f, m_SoundChannel);
	}
	else
	{
		dae::ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(bomberman::SoundId::WalkVertical), 1.0f, m_SoundChannel);
	}
}
