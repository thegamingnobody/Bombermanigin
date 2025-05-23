#pragma once
#include <Command.h>

namespace bomberman
{
    class SkipLevelCommand final : public dae::Command
    {
    public:
        SkipLevelCommand() = default;

        void Execute() override;
    };
}

