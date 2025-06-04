#pragma once

#include <Command.h>
#include <GameObject.h>

namespace bomberman
{
	class CursorSelectCommand : public dae::Command
	{
	public:
		CursorSelectCommand(dae::GameObject& controllingObject);

		void Execute() override;

	private:
		dae::GameObject* m_pControllingObject;
	};
}

