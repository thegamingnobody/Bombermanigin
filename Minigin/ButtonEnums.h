#ifndef BUTTONENUMS
#define BUTTONENUMS

namespace dae
{
	enum class InputType
	{
		PressedThisFrame,
		Held,
		ReleasedThisFrame
	};

	enum class GamepadButtons
	{
		DpadUp = 0x0001,
		DpadDown = 0x0002,
		DpadLeft = 0x0004,
		DpadRight = 0x0008,

		Start = 0x0010,
		Select = 0x0020,
		StickLeft = 0x0040,
		StickRight = 0x0080,

		ShoulderLeft = 0x0100,
		ShoulderRight = 0x0200,

		FaceButtonDown = 0x1000,
		FaceButtonRight = 0x2000,
		FaceButtonLeft = 0x4000,
		FaceButtonUp = 0x8000,
	};

}

#endif