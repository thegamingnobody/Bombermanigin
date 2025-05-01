#include "TimeManager.h"
#include <json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void dae::TimeManager::Init()
{
	std::filesystem::path filePath = __FILE__;
	std::filesystem::path fileDir = filePath.parent_path();

	auto configPath = "Config\\config.json";

	fileDir.append(configPath);
	std::ifstream f(fileDir.c_str());
	std::ifstream f2(configPath);
	json data;
	try
	{
		data = json::parse(f);
	}
	catch (...)
	{
		data = json::parse(f2);
	}

	int const targetFramerate{ data["targetFramerate"] };
	m_FixedTimeStep = data["fixedTimeStep"];
	m_MsPerFrame = 1000 / targetFramerate;

	m_LastTime = std::chrono::high_resolution_clock::now();
}

void dae::TimeManager::Update()
{
	m_CurrentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_LastTime).count();
	m_LastTime = m_CurrentTime;
}

std::chrono::milliseconds dae::TimeManager::GetSleepTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(m_MsPerFrame) - (std::chrono::high_resolution_clock::now() - m_CurrentTime));
}
