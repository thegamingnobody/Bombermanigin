#include "CursorSelectCommand.h"
#include "StartGameEvent.h"
#include <EventManager.h>
#include <SceneManager.h>
#include "SceneNames.h"


bomberman::CursorSelectCommand::CursorSelectCommand()
{
}

void bomberman::CursorSelectCommand::Execute()
{
	dae::EventManager::GetInstance().BroadcastEvent(std::make_unique<bomberman::StartGameEvent>());
	dae::SceneManager::GetInstance().SetSceneActive(SCENE_MAIN_MENU, false);
}
