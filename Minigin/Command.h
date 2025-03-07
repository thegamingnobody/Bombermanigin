#ifndef DAE_COMMAND
#define DAE_COMMAND

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}

#endif