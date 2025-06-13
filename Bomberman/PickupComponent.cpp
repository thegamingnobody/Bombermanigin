#include "PickupComponent.h"

bomberman::PickupComponent::PickupComponent(dae::GameObject& owner, std::shared_ptr<PickupBase> pickup)
	: dae::Component(owner)
	, m_Pickup(nullptr)
{
	m_Pickup = pickup;
}

void bomberman::PickupComponent::CollectPickup(int playerId)
{
	if (!m_Pickup)
		return;

	m_Pickup->CollectPickup(playerId);
}