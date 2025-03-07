#ifndef GAMEPADDEVICE
#define GAMEPADDEVICE

#include "InputDeviceBase.h"
#include <memory>

namespace dae
{
    class GamepadDevice : public InputDeviceBase
    {
	public:
		GamepadDevice(int const index);
		~GamepadDevice();

		void Update() override;

		bool IsButtonPressed(int const button, const InputType& inputType) const override;

		int GetDeviceID() const;

		bool IsInUse() const;

		void SetInUse(bool newUseState);

	private:
		class GamepadImpl;
		std::unique_ptr<GamepadImpl> m_Impl;
	};
}

#endif