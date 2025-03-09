#pragma once
#include "InputDeviceBase.h"
#include <memory>

namespace dae
{
    class KeyboardDevice final : public InputDeviceBase
    {
	public:

		KeyboardDevice(int const index);
		~KeyboardDevice();

		void Update() override;

		bool IsButtonPressed(int const button, const InputType& inputType) const override;

		int GetDeviceID() const;

		bool IsInUse() const;

		void SetInUse(bool newUseState);

	private:
		class KeyboardImpl;
		std::unique_ptr<KeyboardImpl> m_Impl;

    };
}

