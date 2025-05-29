#include "MenuState.h"
#include "StateMachineBase.h"
#include <SceneManager.h>
#include "SceneNames.h"

bomberman::MenuState::MenuState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::MenuState::OnEnter()
{
	auto& sceneManager = dae::SceneManager::GetInstance();

	sceneManager.SetSceneActive(SCENE_MAP, false);
	sceneManager.SetSceneActive(SCENE_OBJECTS, false);
	sceneManager.SetSceneActive(SCENE_PLAYERS, false);
	sceneManager.SetSceneActive(SCENE_HUD, false);
}

void bomberman::MenuState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::MenuState::Update(float /*deltaTime*/)
{
	return nullptr;
}
