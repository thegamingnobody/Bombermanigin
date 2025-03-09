#ifndef DAE_ACTION
#define DAE_ACTION

#include "ButtonEnums.h"
#include <memory>
#include "Command.h"

namespace dae
{
	class Action
	{
	public:
		enum class DeviceType
		{
			Gamepad,
			Keyboard
		};

		Action(const GamepadButtons& gamepadButton, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID);
		Action(const KeyboardKeys& keyboardKey, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID);

		void Execute();

		int GetButton()				const { return m_ButtonValue; }
		int GetDeviceID()			const { return m_DeviceID; }
		DeviceType GetDeviceType()	const { return m_DeviceType; }
		InputType GetInputType()	const { return m_InputType; }


	private:
		int m_DeviceID;

		int m_ButtonValue;
		std::shared_ptr<Command> m_Command;
		DeviceType m_DeviceType;
		InputType m_InputType;

	};
}

#endif