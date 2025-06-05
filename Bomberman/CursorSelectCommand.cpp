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
	int selectedOption = static_cast<int>(cursorPos.y / TILE_SIZE);

	switch (static_cast<CursorOptions>(selectedOption))
	{
	case CursorOptions::SinglePlayer:
		dae::EventManager::GetInstance().BroadcastEvent(std::make_unique<bomberman::StartGameEvent>(GameMode::Singleplayer));
		dae::SceneManager::GetInstance().SetSceneActive(SCENE_MAIN_MENU, false);
		break;
	case CursorOptions::Coop:
		dae::EventManager::GetInstance().BroadcastEvent(std::make_unique<bomberman::StartGameEvent>(GameMode::Coop));
		dae::SceneManager::GetInstance().SetSceneActive(SCENE_MAIN_MENU, false);
		break;
	case CursorOptions::Versus:
		dae::EventManager::GetInstance().BroadcastEvent(std::make_unique<bomberman::StartGameEvent>(GameMode::Versus));
		dae::SceneManager::GetInstance().SetSceneActive(SCENE_MAIN_MENU, false);
		break;
	case CursorOptions::Quit:
	{
		// Todo: is this ok?
		SDL_Event quitEvent;
		quitEvent.type = SDL_QUIT;
		SDL_PushEvent(&quitEvent);
	}
		break;
	default:
		break;
	}

}
