#ifndef DAE_ATTACKCOMMAND
#define DAE_ATTACKCOMMAND

#include "Command.h"
#include <GameObject.h>
#include <Observer.h>

namespace bomberman
{
    class AttackCommand final : public dae::Command, public dae::Observer
    {
	public:
		AttackCommand(dae::GameObject& controllingObject);

		void Execute() override;

		void Notify(const dae::Event& event) override;

	private:
		dae::GameObject* m_pControllingObject;

		bool m_CanSpawnBomb{ true };

		void SpawnBombObject(glm::vec3 position);
    };
}

#endif