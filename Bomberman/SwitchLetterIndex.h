#pragma once
#include <Command.h>
namespace bomberman
{
	class SwitchLetterIndex : public dae::Command
	{
	public:
		SwitchLetterIndex(bool moveRight);

		void Execute() override;

	private:

		bool m_MoveRight;
	};
}

