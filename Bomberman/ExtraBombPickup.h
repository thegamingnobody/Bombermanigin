#pragma once
#include "PickupBase.h"

namespace bomberman
{
    class ExtraBombPickup : public PickupBase
    {
    public:
        ExtraBombPickup() = default;
		~ExtraBombPickup() override = default;

		void CollectPickup(int playerId) override;
    };
}

