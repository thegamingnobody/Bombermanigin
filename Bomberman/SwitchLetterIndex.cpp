#include "SwitchLetterIndex.h"
#include "NameEntryManager.h"

bomberman::SwitchLetterIndex::SwitchLetterIndex(bool moveRight)
	: m_MoveRight(moveRight)
{
}

void bomberman::SwitchLetterIndex::Execute()
{
	auto& nameEntryManager = bomberman::NameEntryManager::GetInstance();

	if (m_MoveRight)
	{
		nameEntryManager.MoveRight();
	}
	else
	{
		nameEntryManager.MoveLeft();
	}

}
