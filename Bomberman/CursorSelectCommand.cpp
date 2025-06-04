#include "CursorSelectCommand.h"
#include "StartGameEvent.h"
#include <EventManager.h>
#include <SceneManager.h>
#include "SceneNames.h"
#include "Grid.h"
#include "CursorMoveCommand.h"
#include <SDL.h>


bomberman::CursorSelectCommand::CursorSelectCommand(dae::GameObject& controllingObject)
	: m_pControllingObject(&controllingObject)
{
}

void bomberman::CursorSelectCommand::Execute()
{
	auto cursorPos = m_pControllingObject->GetTransform()->GetGlobalPosition();

	if (cursorPos.y == (TILE_SIZE * static_cast<int>(CursorOptions::Quit)))
	{
		// Todo: is this ok?
		SDL_Event quitEvent;
		quitEvent.type = SDL_QUIT;
		SDL_PushEvent(&quitEvent);
	}
	else
	{
		dae::EventManager::GetInstance().BroadcastEvent(std::make_unique<bomberman::StartGameEvent>());
		dae::SceneManager::GetInstance().SetSceneActive(SCENE_MAIN_MENU, false);
	}
}
