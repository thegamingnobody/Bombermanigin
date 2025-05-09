#include "DAE_SDL_Soundsystem.h"

#include <iostream>
#include <queue>
#include <mutex>
#include <SDL_mixer.h>
#include <unordered_map>
#include <filesystem>
#include <condition_variable>
#include <thread>
#include <stdexcept>
#include <string>
#include <functional>


class dae::DAE_SDL_Soundsystem::SDLSoundImpl
{
public:
	SDLSoundImpl()
		: m_SoundThread()
		, m_SoundQueue()
		, m_Mutex()
		, m_Sounds()
		, m_RunThread(true)
		, m_IsMuted(false)
	{
		StartUp();
	}

	~SDLSoundImpl()
	{
		Mix_HaltChannel(-1);

		for (auto& sound : m_Sounds)
		{
			Mix_FreeChunk(sound.second.m_Sound);
			sound.second.isLoaded = false;
		}

		{
			std::scoped_lock<std::mutex> lock(m_Mutex);
			std::queue<SoundInfo> emptyQueue;
			std::swap(m_SoundQueue, emptyQueue);
		}

		QuitThread();

		Mix_CloseAudio();
		Mix_Quit();
	}

	void PlaySound(const SoundId soundId, const float volume, int const channel = -1)
	{
		SoundInfo soundInfo{ soundId, volume, channel };

		{
			std::scoped_lock<std::mutex> lock(m_Mutex);
			m_SoundQueue.push(soundInfo);
		}

		m_ConditionVar.notify_one();
	}
	void StopSound(const SoundId)
	{
		//todo: do
	}
	void StopAllSounds()
	{
		//todo: do
	}

	void ProcessQueue()
	{
		while (m_RunThread)
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_ConditionVar.wait(lock, [&] { return !m_SoundQueue.empty() or !m_RunThread; });

			if (not m_RunThread and m_SoundQueue.empty()) break;

			auto soundRequest{ m_SoundQueue.front() };
			m_SoundQueue.pop();

			lock.unlock();

			ProcessSound(soundRequest);
		}

		std::cout << "Sound thread exiting\n";
	}

	void QuitThread()
	{
		{
			std::scoped_lock<std::mutex> lock(m_Mutex);
			m_RunThread = false;
			m_ConditionVar.notify_all();
		}
	}

	void AddSound(const SoundId soundId, const std::string& filePath)
	{
		SDL_SoundInfo sound;
		sound.m_SoundPath = filePath;
		sound.isLoaded = false;

		m_Sounds[soundId] = sound;
	}

	void ToggleMute()
	{
		m_IsMuted = !m_IsMuted;
	}

private:
	void StartUp()
	{
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) != 0)
		{
			std::cout << "Unable to open Audio Device, Mix error: " << Mix_GetError() << "\n";
			throw std::runtime_error("Unable to open Audio Device");
		}
		std::cout << "Opened Audio Device\n";

		m_SoundThread = std::jthread(&SDLSoundImpl::ProcessQueue, this);
	}

	struct SDL_SoundInfo
	{
		//Todo: add looping sounds
		std::string m_SoundPath;
		Mix_Chunk* m_Sound{ nullptr };
		bool isLoaded{ false };

		SDL_SoundInfo() = default;
	};
	
	void ProcessSound(SoundInfo soundInfo)
	{
		SDL_SoundInfo sound = GetSound(soundInfo.m_SoundId);
	
		if (m_IsMuted) return;

		if (soundInfo.m_Channel != -1)
		{
			//checks if specified channel is playing
			if (Mix_Playing(soundInfo.m_Channel) != 0)
				return;
		}

		Mix_VolumeChunk(sound.m_Sound, static_cast<int>(soundInfo.m_Volume * MIX_MAX_VOLUME));
		Mix_PlayChannel(soundInfo.m_Channel, sound.m_Sound, 0);
	}

	SDL_SoundInfo GetSound(SoundId soundID)
	{
		auto it = m_Sounds.find(soundID);
		if (it != m_Sounds.end() and it->second.isLoaded)
			return it->second;

		//First one is for when running in Visual Studio, the second one is for the build
		//Could maybe use a better solution but haven't come up with one yet
		std::filesystem::path filePath1{ "../Data/" + m_Sounds[soundID].m_SoundPath };
		std::filesystem::path filePath2{ "Data/" + m_Sounds[soundID].m_SoundPath };
		std::string actualPath{};

		if (std::filesystem::exists(filePath1))
		{
			actualPath = filePath1.string();
		}
		else if (std::filesystem::exists(filePath2))
		{
			actualPath = filePath2.string();
		}
		else
		{
			throw std::runtime_error("File not found: " + actualPath);
		}

		Mix_Chunk* chunk = Mix_LoadWAV(actualPath.c_str());

		if (chunk)
		{
			if (m_Sounds[soundID].isLoaded && m_Sounds[soundID].m_Sound != nullptr)
			{
				Mix_FreeChunk(m_Sounds[soundID].m_Sound);
			}

			m_Sounds[soundID].m_Sound = chunk;
			m_Sounds[soundID].isLoaded = true;
			return m_Sounds[soundID];
		}
		else
		{
			std::cout << "Failed to load sound: " << actualPath << ": " << Mix_GetError() << "\n";
			return SDL_SoundInfo();
		}
	}

	std::jthread m_SoundThread;
	std::queue<SoundInfo> m_SoundQueue;
	std::mutex m_Mutex;
	std::unordered_map<SoundId, SDL_SoundInfo> m_Sounds{};
	std::condition_variable m_ConditionVar;
	bool m_RunThread;
	bool m_IsMuted;
};


dae::DAE_SDL_Soundsystem::DAE_SDL_Soundsystem()
{
	m_Impl = std::make_unique<SDLSoundImpl>();
}

dae::DAE_SDL_Soundsystem::~DAE_SDL_Soundsystem()
{}

void dae::DAE_SDL_Soundsystem::PlaySound(const SoundId soundId, const float volume, int const channel)
{
	m_Impl->PlaySound(soundId, volume, channel);
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

void dae::DAE_SDL_Soundsystem::ToggleMute()
{
	m_Impl->ToggleMute();
}
