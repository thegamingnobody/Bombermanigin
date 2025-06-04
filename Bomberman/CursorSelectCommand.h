#pragma once

#include <Command.h>
#include <GameObject.h>

namespace bomberman
{
	class CursorSelectCommand : public dae::Command
	{
	public:
		CursorSelectCommand();

		void Execute() override;
	};
}

