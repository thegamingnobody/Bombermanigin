#include "ResetLevelCommand.h"
#include "GameManager.h"
#include "ResetLevelEvent.h"
#include <EventManager.h>

void bomberman::ResetLevelCommand::Execute() 
{
	bomberman::GameManager::GetInstance().ResetLevel();
	bomberman::ResetLevelEvent event;
	dae::EventManager::GetInstance().BroadcastEvent(std::make_unique<bomberman::ResetLevelEvent>(event));
}
