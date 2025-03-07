#pragma once
#include "Command.h"
#include <glm.hpp>
#include "GameObject.h"

namespace dae
{
    class MoveCommand final : public Command
    {
	public:
		MoveCommand(GameObject& controllingObject, glm::vec3 direction);

		void Execute() override;

	private:
		GameObject* m_pControllingObject;
		const glm::vec3 m_Direction;

    };
}
