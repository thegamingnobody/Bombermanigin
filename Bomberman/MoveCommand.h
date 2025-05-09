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
		void SetDirection(glm::vec3 direction) { m_Direction = direction; }

	private:
		dae::GameObject* m_pControllingObject;
		glm::vec3 m_Direction;
		//todo: find better solution?
		int const m_SoundChannel{ 0 };

    };
}

#endif