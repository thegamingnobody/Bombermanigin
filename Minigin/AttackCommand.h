#ifndef DAE_ATTACKCOMMAND
#define DAE_ATTACKCOMMAND

#include "Command.h"
#include "GameObject.h"

namespace dae
{
    class AttackCommand final : public Command
    {
	public:
		AttackCommand(GameObject& controllingObject);

		void Execute() override;

	private:
		GameObject* m_pControllingObject;
    };
}

#endif