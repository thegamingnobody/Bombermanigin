#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
//#include <iostream>
#pragma comment(lib, "xinput.lib")

#include "GamepadDevice.h"


//*-----------------------------------------*
//|				 Implementation				|
//*-----------------------------------------*
class dae::GamepadDevice::GamepadImpl
{
public:
	GamepadImpl(int const index)
		: m_DeviceID(index)
		, m_LastState()
		, m_CurrentState()
		, m_IsInUse(false)
	{
		ZeroMemory(&m_LastState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	}

	void Update()
	{
		//todo: detect if controller disconected?
		CopyMemory(&m_LastState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		/*auto result = */XInputGetState(m_DeviceID, &m_CurrentState);
	}

	bool IsButtonPressed(int const button, const InputType& inputType) const
	{
		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_LastState.Gamepad.wButtons;
		auto buttonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		auto buttonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
		switch (inputType)
		{
		case dae::InputType::PressedThisFrame:
			return (buttonsPressedThisFrame & button);
		case dae::InputType::Held:
			return (m_CurrentState.Gamepad.wButtons & button);
		case dae::InputType::ReleasedThisFrame:
			return (buttonsReleasedThisFrame & button);
		default:
			return false;
		}
	}

	int GetDeviceID() const
	{
		return m_DeviceID;
	}

	bool IsInUse() const
	{
		return m_IsInUse;
	}

	void SetInUse(bool newUseState)
	{
		m_IsInUse = newUseState;
	}

private:
	int m_DeviceID{};
	XINPUT_STATE m_LastState;
	XINPUT_STATE m_CurrentState;
	bool m_IsInUse;
};

//*-----------------------------------------*
//|			   Wrapper Functions			|
//*-----------------------------------------*
dae::GamepadDevice::GamepadDevice(int const index)
{
	m_Impl = std::make_unique<GamepadImpl>(index);
}

dae::GamepadDevice::~GamepadDevice()
{
}

void dae::GamepadDevice::Update()
{
	m_Impl->Update();
}

bool dae::GamepadDevice::IsButtonPressed(int const button, const InputType& inputType) const
{
	return m_Impl->IsButtonPressed(button, inputType);
}

int dae::GamepadDevice::GetDeviceID() const
{
	return m_Impl->GetDeviceID();
}

bool dae::GamepadDevice::IsInUse() const
{
	return m_Impl->IsInUse();
}

void dae::GamepadDevice::SetInUse(bool newUseState)
{
	m_Impl->SetInUse(newUseState);
}
