#include "ResetLevelCommand.h"
#include "GameManager.h"

void bomberman::ResetLevelCommand::Execute() 
{
	bomberman::GameManager::GetInstance().ResetLevel();
}
