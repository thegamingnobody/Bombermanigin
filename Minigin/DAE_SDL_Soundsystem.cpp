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
			Mix_FreeChunk(sound.second);
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

private:
	void ProcessSound(SoundInfo soundInfo)
	{
		Mix_Chunk* sound = GetSound(soundInfo.m_SoundId);

		if (Mix_Playing(m_SoundChannel) == 0)
		{
			Mix_VolumeChunk(sound, static_cast<int>(soundInfo.m_Volume * MIX_MAX_VOLUME));
			Mix_PlayChannel(m_SoundChannel, sound, 0);
			//Mix_FreeChunk(m_Chunk);
		}

	}

	Mix_Chunk* GetSound(SoundId soundID)
	{
		auto it = m_Sounds.find(soundID);
		if (it != m_Sounds.end())
			return it->second;

		//todo: file path generator based on sound id (user geeft map mee met de id en een std string file naam?
		std::string filePath{ "../Data/Walk.wav" };
		Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());

		if (sound)
		{
			m_Sounds[soundID] = sound;
			return sound;
		}
		else
		{
			std::cout << "Failed to load sound: " << filePath << ": " << Mix_GetError() << "\n";
			return nullptr;
		}
	}


	std::jthread m_SoundThread;
	std::queue<SoundInfo> m_SoundQueue;
	std::mutex m_Mutex;
	bool m_RunThread;
	int m_SoundChannel{ 1 };
	std::unordered_map<SoundId, Mix_Chunk*> m_Sounds{};
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
