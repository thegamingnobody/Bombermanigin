#include "PickupComponent.h"
#include <ServiceLocator.h>
#include "SoundIds.h"

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

	float volume = 0.15f;
	dae::ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(bomberman::SoundId::Pickup), volume, -1);

}