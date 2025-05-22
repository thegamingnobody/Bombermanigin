#include "SkipLevelCommand.h"
#include "GameManager.h"

void bomberman::SkipLevelCommand::Execute()
{
	bomberman::GameManager::GetInstance().LoadNextLevel();
}