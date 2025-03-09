#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>

#include "KeyboardDevice.h"

//*-----------------------------------------*
//|				 Implementation				|
//*-----------------------------------------*
class dae::KeyboardDevice::KeyboardImpl
{
public:
	KeyboardImpl(int const index)
		: m_DeviceID(index)
		, m_LastState()
		, m_CurrentState()
		, m_IsInUse(false)
	{
		ZeroMemory(&m_CurrentState, sizeof(Uint8*));
		auto currentState{ SDL_GetKeyboardState(nullptr) };
		CopyMemory(&m_CurrentState, &currentState, sizeof(Uint8*));
	}

	void Update()
	{
		CopyMemory(m_LastState, m_CurrentState, SDL_NUM_SCANCODES * sizeof(Uint8));
		SDL_PumpEvents();
	}

	bool IsButtonPressed(int const button, const InputType& inputType) const
	{
		switch (inputType)
		{
		case dae::InputType::PressedThisFrame:
			return (m_CurrentState[button] and not m_LastState[button]);
		case dae::InputType::Held:
			return (m_CurrentState[button] and m_LastState[button]);
		case dae::InputType::ReleasedThisFrame:
			return (not m_CurrentState[button] and m_LastState[button]);
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
	Uint8 m_LastState[SDL_NUM_SCANCODES];
	Uint8* m_CurrentState;
	bool m_IsInUse;

};



dae::KeyboardDevice::KeyboardDevice(int const index)
{
	m_Impl = std::make_unique<KeyboardImpl>(index);
}

dae::KeyboardDevice::~KeyboardDevice()
{
}

void dae::KeyboardDevice::Update()
{
	m_Impl->Update();
}

bool dae::KeyboardDevice::IsButtonPressed(int const button, const InputType& inputType) const
{
	return m_Impl->IsButtonPressed(button, inputType);
}

int dae::KeyboardDevice::GetDeviceID() const
{
	return m_Impl->GetDeviceID();
}

bool dae::KeyboardDevice::IsInUse() const
{
	return m_Impl->IsInUse();
}

void dae::KeyboardDevice::SetInUse(bool newUseState)
{
	m_Impl->SetInUse(newUseState);
}