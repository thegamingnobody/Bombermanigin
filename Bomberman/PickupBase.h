#pragma once

namespace bomberman
{
	class PickupBase
	{
	public:
		PickupBase() = default;
		virtual ~PickupBase() = default;

		virtual void CollectPickup(int /*playerId*/) {};
	};
}

