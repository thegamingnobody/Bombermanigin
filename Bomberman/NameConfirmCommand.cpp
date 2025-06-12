#include "NameConfirmCommand.h"
#include "NameEntryManager.h"

bomberman::NameConfirmCommand::NameConfirmCommand()
{
}

void bomberman::NameConfirmCommand::Execute()
{
	bomberman::NameEntryManager::GetInstance().ConfirmName();
}
