#ifndef COMMANDBASE
#define COMMANDBASE

namespace dae
{
	class CommandBase
	{
	public:
		virtual ~CommandBase() = default;
		virtual void Excecute() = 0;
	};
}

#endif