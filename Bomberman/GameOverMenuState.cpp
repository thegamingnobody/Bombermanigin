#include "GameOverMenuState.h"
#include <SceneManager.h>
#include "SceneNames.h"

bomberman::GameOverMenuState::GameOverMenuState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::GameOverMenuState::OnEnter()
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	sceneManager.SetSceneActive(SCENE_MAP, false);
	sceneManager.SetSceneActive(SCENE_OBJECTS, false);
	sceneManager.SetSceneActive(SCENE_PLAYERS, false);
	sceneManager.SetSceneActive(SCENE_HUD, false);
}

void bomberman::GameOverMenuState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::GameOverMenuState::Update(float /*deltaTime*/)
{
	return nullptr;
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::GameOverMenuState::Notify(const dae::Event& /*event*/)
{
	return nullptr;
}
