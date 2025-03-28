#include "TimeManager.h"
#include <json.hpp>
#include <fstream>

using json = nlohmann::json;

void dae::TimeManager::Init()
{
	std::ifstream f("../config.json");
	json data = json::parse(f);

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
