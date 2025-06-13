#include <SDL.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <Xinput.h>
#include <winerror.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <stdexcept>
#include "KeyboardDevice.h"
#include "ServiceLocator.h"

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
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) 
		{
			return false;
		}
        else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F2)
		{
			dae::ServiceLocator::GetSoundSystem().ToggleMute();
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
	case Action::DeviceType::UnUsed:
		return -1;
	}
	m_InputDevices[result]->SetInUse(true);
	return result;
}

void dae::InputManager::RemoveInputDevice(const Action::DeviceType& deviceType)
{
	int index{-1};
	switch (deviceType)
	{
	case Action::DeviceType::Gamepad:
		for (int i = 3; i >= 0; --i)
		{
			if (!m_InputDevices[i]->IsInUse()) continue;

			index = i;
		}
		break;
	case Action::DeviceType::Keyboard:
		index = m_KeyboardId;
		break;
	}

	if (index != -1)
	{
		m_InputDevices[index]->SetInUse(false);

		m_Actions.erase(
			std::remove_if(m_Actions.begin(), m_Actions.end(), [index](const std::unique_ptr<Action>& action)
				{
					return action && action->GetDeviceID() == index;
				}),
			m_Actions.end()
		);
	}
}

void dae::InputManager::AddAction(const GamepadButtons& gamepadButton, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID)
{
	auto it = std::find_if(m_Actions.begin(), m_Actions.end(), [gamepadButton, deviceID](const std::unique_ptr<dae::Action>& action)
		{
			return ((action->GetButton() == static_cast<int>(gamepadButton)) and (action->GetDeviceID() == deviceID));
		});

	if (it == m_Actions.end())
	{
		//only add if button is not already present
		m_Actions.emplace_back(std::make_unique<Action>(gamepadButton, inputType, command, deviceID));
	}

}

void dae::InputManager::AddAction(const KeyboardKeys& keyboardKey, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID)
{
	// Check if the action already exists for the given key/button and device ID
	auto it = std::find_if(m_Actions.begin(), m_Actions.end(), [keyboardKey](const std::unique_ptr<dae::Action>& action)
		{
			return (action->GetButton() == static_cast<int>(keyboardKey));
		});

	if (it == m_Actions.end())
	{
		//only add if button is not already present
		m_Actions.emplace_back(std::make_unique<Action>(keyboardKey, inputType, command, deviceID));
	}
}

void dae::InputManager::RemoveAction(const GamepadButtons& gamepadButton, int const deviceID)
{
	m_Actions.erase(std::remove_if(m_Actions.begin(), m_Actions.end(), [gamepadButton, deviceID](const std::unique_ptr<dae::Action>& action)
		{
			return ((action->GetButton() == static_cast<int>(gamepadButton)) && (action->GetDeviceID() == deviceID));
		}),
		m_Actions.end());
}

void dae::InputManager::RemoveAction(const KeyboardKeys& keyboardKey, int const deviceID)
{
	m_Actions.erase(std::remove_if(m_Actions.begin(), m_Actions.end(), [keyboardKey, deviceID](const std::unique_ptr<dae::Action>& action)
		{
			return ((action->GetButton() == static_cast<int>(keyboardKey)) && (action->GetDeviceID() == deviceID));
		}),
		m_Actions.end());
}

bool dae::InputManager::IsDeviceConnected(int const deviceID) const
{
	XINPUT_STATE state = {};
	DWORD result = XInputGetState(deviceID, &state);
	return (result == ERROR_SUCCESS);
}

int dae::InputManager::GetAvailableGamepadIndex()
{
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
