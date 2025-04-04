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


		//Todo: Add Remove input device
		int AddInputDevice(const Action::DeviceType& deviceType);

		//Todo: Add Remove action
		//Todo: moet command een shared pointer zijn?
		void AddAction(const GamepadButtons& gamepadButton, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID);
		void AddAction(const KeyboardKeys& keyboardKey, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID);


		

	private:
		int GetAvailableGamepadIndex();
		std::vector<std::unique_ptr<InputDeviceBase>> m_InputDevices;
		std::vector<std::unique_ptr<Action>> m_Actions;
		int const m_KeyboardId{4};
	};

}

#endif