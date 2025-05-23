#pragma once
#include <Command.h>

namespace bomberman
{
	class ResetLevelCommand : public dae::Command
	{
	public:
		ResetLevelCommand() = default;
		
		void Execute() override;
	};
}