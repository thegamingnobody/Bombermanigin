#ifndef DAE_ATTACKCOMMAND
#define DAE_ATTACKCOMMAND

#include "Command.h"
#include "GameObject.h"

namespace bomberman
{
    class AttackCommand final : public dae::Command
    {
	public:
		AttackCommand(dae::GameObject& controllingObject);

		void Execute() override;

	private:
		dae::GameObject* m_pControllingObject;

		void SpawnBombObject(glm::vec3 position);
    };
}

#endif