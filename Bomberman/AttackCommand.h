#ifndef ATTACKCOMMAND
#define ATTACKCOMMAND

#include "Command.h"
#include <GameObject.h>
#include <Observer.h>

namespace bomberman
{
    class AttackCommand final : public dae::Command, public dae::Observer
    {
	public:
		AttackCommand(dae::GameObject& controllingObject, int playerNumber);

		void Execute() override;

		void Notify(const dae::Event& event) override;

	private:
		dae::GameObject* m_pControllingObject;

		int m_BombCount{ 0 };
		int m_PlayerNumber{ -1 };

		void SpawnBombObject(glm::vec3 position);
    };
}

#endif