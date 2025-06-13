#include "ExtraBombPickup.h"
#include "PlayerManager.h"

void bomberman::ExtraBombPickup::CollectPickup(int playerId)
{
	bomberman::PlayerManager::GetInstance().GetPlayerInfo(playerId).maxBombs++;
}
