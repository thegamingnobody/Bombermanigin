#pragma once
#include <Command.h>
#include <GameObject.h>

namespace bomberman
{
	class IncDecLetterCommand : public dae::Command
	{
	public:
		IncDecLetterCommand(bool increment);

		void Execute() override;

	private:

		bool m_Increment;
	};
}


