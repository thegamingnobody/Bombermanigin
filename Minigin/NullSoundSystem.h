#ifndef DAE_NULLSOUNDSYSTEM
#define DAE_NULLSOUNDSYSTEM

#include "SoundSystemBase.h"

namespace dae
{
	class NullSoundSystem final : public SoundSystemBase
	{
	public:
		NullSoundSystem() = default;
		~NullSoundSystem() override = default;

		NullSoundSystem(const NullSoundSystem& other) = delete;
		NullSoundSystem(NullSoundSystem&& other) = delete;
		NullSoundSystem& operator=(const NullSoundSystem& other) = delete;
		NullSoundSystem& operator=(NullSoundSystem&& other) = delete;

		void PlaySound(const SoundId, const float) override {};
		void StopSound(const SoundId) override {};
		void StopAllSounds() override {};
	};
}

#endif