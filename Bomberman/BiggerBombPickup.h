#pragma once
#include "PickupBase.h"

namespace bomberman
{
    class BiggerBombPickup : public PickupBase
    {
    public:
        BiggerBombPickup() = default;
        ~BiggerBombPickup() override = default;

        void CollectPickup(int playerId) override;
    };
}


