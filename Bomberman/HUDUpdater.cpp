#include "HUDUpdater.h"
#include "HUDManager.h"

bomberman::HUDUpdater::HUDUpdater(dae::GameObject& object)
	: dae::Component(object)
{
}


void bomberman::HUDUpdater::Update(float /*deltaTime*/)
{
	bomberman::HUDManager::GetInstance().Update();
}