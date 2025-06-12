#pragma once
#include <Command.h>

namespace bomberman
{
    class NameConfirmCommand : public dae::Command
    {
    public:
        NameConfirmCommand();
        ~NameConfirmCommand() override = default;

        void Execute() override;
    };

}


