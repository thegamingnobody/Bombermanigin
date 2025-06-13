#ifndef DAE_INPUTMANAGER
#define DAE_INPUTMANAGER

#include "Singleton.h"
#include <vector>
#include <memory>
#include "GamepadDevice.h"
#include "Action.h"
#include "Command.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		void Init();

		bool ProcessInput();


		int AddInputDevice(const Action::DeviceType& deviceType);
		void RemoveInputDevice(const Action::DeviceType& deviceType);

		void AddAction(const GamepadButtons& gamepadButton, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID);
		void AddAction(const KeyboardKeys& keyboardKey, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID);

		void RemoveAction(const GamepadButtons& gamepadButton, int const deviceID);
		void RemoveAction(const KeyboardKeys& keyboardKey, int const deviceID);

		bool IsDeviceConnected(int const deviceID) const;

	private:
		int GetAvailableGamepadIndex();
		std::vector<std::unique_ptr<InputDeviceBase>> m_InputDevices;
		std::vector<std::unique_ptr<Action>> m_Actions;
		int const m_KeyboardId{4};
	};

}

#endif