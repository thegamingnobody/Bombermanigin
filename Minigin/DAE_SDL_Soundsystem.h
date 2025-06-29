#ifndef SDL_SOUNDSYSTEM
#define SDL_SOUNDSYSTEM

#include "SoundSystemBase.h"
#include <memory>

namespace dae
{
	struct SoundInfo
	{
		SoundInfo(const SoundId& soundId, float const volume, int const channel = -1, bool const loop = false)
			: m_SoundId(soundId)
			, m_Volume(volume)
			, m_Channel(channel)
			, m_Loop(loop)
		{
		}

		SoundId m_SoundId{};
		float m_Volume{};
		int m_Channel{ -1 };
		bool m_Loop{false};

		};

    class DAE_SDL_Soundsystem final : public SoundSystemBase
    {
	public:
		DAE_SDL_Soundsystem();
		~DAE_SDL_Soundsystem();

		DAE_SDL_Soundsystem(const DAE_SDL_Soundsystem& other) = delete;
		DAE_SDL_Soundsystem(DAE_SDL_Soundsystem&& other) = delete;
		DAE_SDL_Soundsystem& operator=(const DAE_SDL_Soundsystem& other) = delete;
		DAE_SDL_Soundsystem& operator=(DAE_SDL_Soundsystem&& other) = delete;

		void PlaySound(const SoundId soundId, const float volume, int const channel = -1, bool loop = false) override;
		void StopSound(const SoundId soundId) override;
		void StopAllSounds() override;

		void QuitThread();

		void AddSound(const SoundId soundId, const std::string& filePath) override;

		void ToggleMute();
	private:
		class SDLSoundImpl;
		std::unique_ptr<SDLSoundImpl> m_Impl;
    };
}

#endif