#include "BiggerBombPickup.h"
#include "PlayerManager.h"

void bomberman::BiggerBombPickup::CollectPickup(int playerId)
{
	auto& playerManager = bomberman::PlayerManager::GetInstance();
	playerManager.GetPlayerInfo(playerId).IncreaseBombRange();
}
