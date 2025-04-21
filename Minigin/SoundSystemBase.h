#ifndef DAE_SOUNDSYSTEM
#define DAE_SOUNDSYSTEM

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

		virtual void PlaySound(const SoundId soundId, const float volume) = 0;
		virtual void StopSound(const SoundId soundId) = 0;
		virtual void StopAllSounds() = 0;
	};

}

#endif
