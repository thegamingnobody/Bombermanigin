#ifndef CURSORMOVECOMMAND
#define CURSORMOVECOMMAND

#include <Command.h>
#include <GameObject.h>

namespace bomberman
{
    enum class CursorOptions
    {
        SinglePlayer    = 9,
		Coop            = 10,
        Versus          = 11,
		Quit            = 12
    };

    class CursorMoveCommand final : public dae::Command
    {
    public:
        CursorMoveCommand(dae::GameObject& controllingObject, bool moveDown);

        void Execute() override;

    private:
        dae::GameObject* m_pControllingObject;
		int m_Direction;
    };
}

#endif