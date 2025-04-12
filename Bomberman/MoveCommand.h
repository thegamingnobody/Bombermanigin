#ifndef BOMBERMAN_MOVECOMMAND
#define BOMBERMAN_MOVECOMMAND

#include "Command.h"
#include <glm.hpp>
#include "GameObject.h"

namespace bomberman
{
    class MoveCommand final : public dae::Command
    {
	public:
		MoveCommand(dae::GameObject& controllingObject, glm::vec3 direction);

		void Execute() override;

	private:
		dae::GameObject* m_pControllingObject;
		const glm::vec3 m_Direction;

    };
}

#endif