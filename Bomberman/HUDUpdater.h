#pragma once
#include <Component.h>

namespace bomberman
{
    class HUDUpdater : public dae::Component
    {
    public:
        HUDUpdater(dae::GameObject& object);
        ~HUDUpdater() override = default;
    
        HUDUpdater(const HUDUpdater& other) = delete;
        HUDUpdater(HUDUpdater&& other) noexcept = delete;
        HUDUpdater& operator=(const HUDUpdater& other) = delete;
        HUDUpdater& operator=(HUDUpdater&& other) noexcept = delete;
        
        void Update(float const deltaTime) override;
    };
}

