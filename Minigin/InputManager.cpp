#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <stdexcept>
//TODO: add commands
void dae::InputManager::Init()
{
	int const maxAantalGamepads{ 4 };
	for (int gamepad = 0; gamepad < maxAantalGamepads; gamepad++)
	{
		m_InputDevices.emplace_back(std::make_unique<GamepadDevice>(gamepad));
	}
}

bool dae::InputManager::ProcessInput()
{
	for (auto& inputDevice : m_InputDevices)
	{
		inputDevice->Update();
		for (auto& action : m_Actions)
		{
			if (not(inputDevice->GetDeviceID() == action->GetDeviceID())) continue;

			if (not(inputDevice->IsButtonPressed(action->GetButton(), action->GetInputType()))) continue;

			action->Execute();
		}
		//inputDevice->IsButtonPressed(static_cast<int>(GamepadButtons::FaceButtonRight), InputType::Held)
		
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

int dae::InputManager::AddInputDevice()
{
	int const availableID{ GetAvailableGamepadIndex() };
	m_InputDevices[availableID]->SetInUse(true);
	return availableID;
}

void dae::InputManager::AddAction(const GamepadButtons& gamepadButton, const InputType& inputType, std::shared_ptr<Command> command, int const deviceID)
{
	m_Actions.emplace_back(std::make_unique<Action>(gamepadButton, inputType, command, deviceID));
}

int dae::InputManager::GetAvailableGamepadIndex()
{
	//todo: kan dit improved worden?
	//todo: keyboard devices moeten een id hebben groter dan 3 => append m_InputDevices?
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
