#ifndef DAE_SOUNDSYSTEM
#define DAE_SOUNDSYSTEM

#include <string>

namespace dae
{
	using SoundId = unsigned short;

	class SoundSystemBase
	{
	public:
		SoundSystemBase() = default;
		virtual ~SoundSystemBase() = default;

		SoundSystemBase(const SoundSystemBase& other) = delete;
		SoundSystemBase(SoundSystemBase&& other) = delete;
		SoundSystemBase& operator=(const SoundSystemBase& other) = delete;
		SoundSystemBase& operator=(SoundSystemBase&& other) = delete;

		virtual void PlaySound(const SoundId soundId, const float volume, int const channel = -1) = 0;
		virtual void StopSound(const SoundId soundId) = 0;
		virtual void StopAllSounds() = 0;

		virtual void AddSound(const SoundId soundId, const std::string& filePath) = 0;

		virtual void ToggleMute() = 0;
	};

}

#endif
