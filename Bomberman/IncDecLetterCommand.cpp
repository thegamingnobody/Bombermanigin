#include "IncDecLetterCommand.h"
#include "NameEntryManager.h"

bomberman::IncDecLetterCommand::IncDecLetterCommand(bool increment)
	: m_Increment(increment)
{
}

void bomberman::IncDecLetterCommand::Execute()
{
	auto& nameEntryManager = bomberman::NameEntryManager::GetInstance();

	if (m_Increment)
	{
		nameEntryManager.IncrementLetter();
	}
	else
	{
		nameEntryManager.DecrementLetter();
	}
}
