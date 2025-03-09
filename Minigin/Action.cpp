#include "Action.h"
#include <iostream>
#include <string>
//TODO: add commands
dae::Action::Action(const GamepadButtons& gamepadButton, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID)
	: m_DeviceID(deviceID)
	, m_ButtonValue(static_cast<int>(gamepadButton))
	, m_Command(command)
	, m_DeviceType(DeviceType::Gamepad)
	, m_InputType(inputType)
{
}

dae::Action::Action(const KeyboardKeys& keyboardKey, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID)
	: m_DeviceID(deviceID)
	, m_ButtonValue(static_cast<int>(keyboardKey))
	, m_Command(command)
	, m_DeviceType(DeviceType::Gamepad)
	, m_InputType(inputType)
{
}

void dae::Action::Execute()
{
	m_Command->Execute();
}