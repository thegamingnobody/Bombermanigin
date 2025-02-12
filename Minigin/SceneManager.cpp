#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float const delta_time)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(delta_time);
	}
}

void dae::SceneManager::FixedUpdate(float const fixed_time_step)
{
	for(auto& scene : m_scenes)
	{
		scene->FixedUpdate(fixed_time_step);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
