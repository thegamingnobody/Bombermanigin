#ifndef INPUTDEVICEBASE
#define INPUTDEVICEBASE

#include "ButtonEnums.h"

namespace dae
{
	class InputDeviceBase
	{
	public:
		virtual ~InputDeviceBase() = default;
		virtual void Update() = 0;
		virtual bool IsButtonPressed(int const button, const InputType& inputType) const = 0;
		virtual int GetDeviceID() const = 0;
		virtual bool IsInUse() const = 0;
		virtual void SetInUse(bool newUseState) = 0;
	};
}
#endif