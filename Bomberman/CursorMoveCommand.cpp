#include "CursorMoveCommand.h"
#include "Grid.h"

bomberman::CursorMoveCommand::CursorMoveCommand(dae::GameObject& controllingObject, bool moveDown)
	: m_pControllingObject(&controllingObject)
{
	// direction is 1 for down, -1 for up
	m_Direction = moveDown ? 1 : -1;
}

void bomberman::CursorMoveCommand::Execute()
{
	auto transform = m_pControllingObject->GetTransform();
	auto cursorPos = transform->GetGlobalPosition();
	float offset = TILE_SIZE * m_Direction;

	if (cursorPos.y + offset < TILE_SIZE * static_cast<int>(CursorOptions::SinglePlayer) 
		or cursorPos.y + offset > TILE_SIZE * static_cast<int>(CursorOptions::Quit))
	{
		// Prevent moving out of bounds
		return;
	}

	transform->Move(0, offset);
}