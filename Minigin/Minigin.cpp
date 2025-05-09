#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <thread>
#include <fstream>
#include <iostream>
#include "TimeManager.h"
#include "EventManager.h"
#include "Camera.h"
#include "SoundSystemBase.h"
#include "ServiceLocator.h"
#include "DAE_SDL_Soundsystem.h"
#include <SDL_mixer.h>

std::unique_ptr<dae::SoundSystemBase> dae::ServiceLocator::m_SoundSystemInstance{ std::make_unique<dae::NullSoundSystem>() };

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	MIX_VERSION(&version);
	printf("We compiled against SDL_Mixer version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *Mix_Linked_Version();
	printf("We are linking against SDL_Mixer version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	Camera::GetInstance().Init();
	glm::vec2 windowSize = Camera::GetInstance().GetWindowSize();
	
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		static_cast<int>(windowSize.x),
		static_cast<int>(windowSize.y),
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	InputManager::GetInstance().Init();

	TimeManager::GetInstance().Init();

	EventManager::GetInstance().Init();

}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& time = dae::TimeManager::GetInstance();
	auto& eventManager = EventManager::GetInstance();
	auto& camera = Camera::GetInstance();

	bool doContinue = true;
	float const fixedTimeStep = time.GetFixedTimeStep();
	float lag = 0.0f;
	while (doContinue)
	{
		time.Update();
		float deltaTime = time.GetDeltaTime();
		lag += deltaTime;

		doContinue = input.ProcessInput();
		while (lag > fixedTimeStep)
		{
			sceneManager.FixedUpdate(fixedTimeStep);
			lag -= fixedTimeStep;
		}

		sceneManager.Update(deltaTime);
		camera.Update();
		
		eventManager.ProcessQueue();

		sceneManager.LateUpdate();

		renderer.Render();

		auto const sleepTime = time.GetSleepTime();
		std::this_thread::sleep_for(sleepTime);
	}
}
