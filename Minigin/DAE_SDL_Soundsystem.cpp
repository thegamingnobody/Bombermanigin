#include "DAE_SDL_Soundsystem.h"

#include <iostream>
#include <queue>
#include <mutex>
#include <SDL_mixer.h>
#include <unordered_map>

class dae::DAE_SDL_Soundsystem::SDLSoundImpl
{
public:
	SDLSoundImpl()
		: m_SoundThread()
		, m_SoundQueue()
		, m_Mutex()
		, m_RunThread(true)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0)
		{
			std::cout << "Opened Audio Device\n";
		}
		else
		{
			std::cout << "Unable to open Audio Device, Mix error: " << Mix_GetError() << "\n";
		}

		m_SoundThread = std::jthread(&SDLSoundImpl::ProcessQueue, this);
	}
	~SDLSoundImpl()
	{
		for (auto& sound : m_Sounds)
		{
			Mix_FreeChunk(sound.second.m_Sound);
		}
		QuitThread();
		Mix_CloseAudio();
	}

	void PlaySound(const SoundId soundId, const float volume)
	{
		SoundInfo soundInfo{ soundId, volume };

		std::scoped_lock<std::mutex> lock(m_Mutex);
		m_SoundQueue.push(soundInfo);
	}
	void StopSound(const SoundId)
	{
	}
	void StopAllSounds()
	{

	}

	void ProcessQueue()
	{
		while (m_RunThread)
		{
			if (not (m_SoundQueue.size() > 0)) continue;

			if (not m_RunThread) continue;

			auto& soundRequest{ m_SoundQueue.front() };
			m_SoundQueue.pop();

			ProcessSound(soundRequest);
		}
	}

	void QuitThread()
	{
		std::scoped_lock<std::mutex> lock(m_Mutex);
		m_RunThread = false;
	}

	void AddSound(const SoundId soundId, const std::string& filePath)
	{
		SDL_SoundInfo sound;
		sound.m_SoundPath = filePath;
		sound.isLoaded = false;

		m_Sounds[soundId] = sound;
	}

private:
	struct SDL_SoundInfo
	{
		std::string m_SoundPath;
		Mix_Chunk* m_Sound{ nullptr };
		bool isLoaded{ false };

		SDL_SoundInfo() = default;
	};
	
	void ProcessSound(SoundInfo soundInfo)
	{
		SDL_SoundInfo sound = GetSound(soundInfo.m_SoundId);

		if (Mix_Playing(m_SoundChannel) == 0)
		{
			Mix_VolumeChunk(sound.m_Sound, static_cast<int>(soundInfo.m_Volume * MIX_MAX_VOLUME));
			Mix_PlayChannel(m_SoundChannel, sound.m_Sound, 0);
		}
	}

	SDL_SoundInfo GetSound(SoundId soundID)
	{
		auto it = m_Sounds.find(soundID);
		if (it != m_Sounds.end() and it->second.isLoaded)
			return it->second;

		//todo: file path generator based on sound id (user geeft map mee met de id en een std string file naam?
		std::string baseFilePath{ "../Data/" };
		std::string filePath = baseFilePath + m_Sounds[soundID].m_SoundPath;

		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

		if (chunk)
		{
			m_Sounds[soundID].m_Sound = chunk;
			m_Sounds[soundID].isLoaded = true;
			return m_Sounds[soundID];
		}
		else
		{
			std::cout << "Failed to load sound: " << filePath << ": " << Mix_GetError() << "\n";
			return SDL_SoundInfo();
		}
	}

	std::jthread m_SoundThread;
	std::queue<SoundInfo> m_SoundQueue;
	std::mutex m_Mutex;
	bool m_RunThread;
	int m_SoundChannel{ 1 };
	std::unordered_map<SoundId, SDL_SoundInfo> m_Sounds{};
};


dae::DAE_SDL_Soundsystem::DAE_SDL_Soundsystem()
{
	m_Impl = std::make_unique<SDLSoundImpl>();
}

dae::DAE_SDL_Soundsystem::~DAE_SDL_Soundsystem()
{
}

void dae::DAE_SDL_Soundsystem::PlaySound(const SoundId soundId, const float volume)
{
	m_Impl->PlaySound(soundId, volume);
}

void dae::DAE_SDL_Soundsystem::StopSound(const SoundId soundId)
{
	m_Impl->StopSound(soundId);
}

void dae::DAE_SDL_Soundsystem::StopAllSounds()
{
	m_Impl->StopAllSounds();
}

void dae::DAE_SDL_Soundsystem::QuitThread()
{
	m_Impl->QuitThread();
}

void dae::DAE_SDL_Soundsystem::AddSound(const SoundId soundId, const std::string& filePath)
{
	m_Impl->AddSound(soundId, filePath);
}
