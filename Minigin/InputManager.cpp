#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <stdexcept>
#include "KeyboardDevice.h"
void dae::InputManager::Init()
{
	int const maxAantalGamepads{ 4 };
	for (int gamepad = 0; gamepad < maxAantalGamepads; gamepad++)
	{
		m_InputDevices.emplace_back(std::make_unique<GamepadDevice>(gamepad));
	}
	//There is only 1 keyboard
	m_InputDevices.emplace_back(std::make_unique<KeyboardDevice>(4));
}

bool dae::InputManager::ProcessInput()
{
	for (auto& inputDevice : m_InputDevices)
	{
		if (not(inputDevice->IsInUse())) continue;

		inputDevice->Update();
		for (auto& action : m_Actions)
		{
			if (not(inputDevice->GetDeviceID() == action->GetDeviceID())) continue;

			if (not(inputDevice->IsButtonPressed(action->GetButton(), action->GetInputType()))) continue;

			action->Execute();
		}		
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}		
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

int dae::InputManager::AddInputDevice(const Action::DeviceType& deviceType)
{
	int result{};
	switch (deviceType)
	{
	case Action::DeviceType::Gamepad:
		result = GetAvailableGamepadIndex();
		break;
	case Action::DeviceType::Keyboard:
		result = m_KeyboardId;
		break;
	}
	m_InputDevices[result]->SetInUse(true);
	return result;
}

void dae::InputManager::AddAction(const GamepadButtons& gamepadButton, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID)
{
	m_Actions.emplace_back(std::make_unique<Action>(gamepadButton, inputType, command, deviceID));
}

void dae::InputManager::AddAction(const KeyboardKeys& keyboardKey, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID)
{
	m_Actions.emplace_back(std::make_unique<Action>(keyboardKey, inputType, command, deviceID));
}

int dae::InputManager::GetAvailableGamepadIndex()
{
	//todo: kan dit improved worden?
	int const maxAantalGamepads{ 4 };
	for (int gamepad = 0; gamepad < maxAantalGamepads; gamepad++)
	{
		if (not m_InputDevices[gamepad]->IsInUse())
		{
			return gamepad;
		}
	}

	throw std::runtime_error("No available Gamepad ports");
}
