#include "MoveCommand.h"
#include "TimeManager.h"
#include "ServiceLocator.h"
#include "DAE_SDL_Soundsystem.h"
#include "ServiceLocator.h"
#include "SoundIds.h"
#include "StateMachineComponent.h"
#include "GameObject.h"

bomberman::MoveCommand::MoveCommand(dae::GameObject& controllingObject, glm::vec3 direction)
	: m_pControllingObject(&controllingObject)
	, m_Direction(direction)
{

}

void bomberman::MoveCommand::Execute()
{
	auto transform = m_pControllingObject->GetTransform();
	auto deltaTime = dae::TimeManager::GetInstance().GetDeltaTime();
	transform->Move(m_Direction * deltaTime);

	float volume = 0.15f;

	if (m_Direction.x != 0.0f)
	{
		dae::ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(bomberman::SoundId::WalkHorizontal), volume, m_SoundChannel);
	}
	else
	{
		dae::ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(bomberman::SoundId::WalkVertical), volume, m_SoundChannel);
	}
}
