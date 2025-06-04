#include "SceneManager.h"
#include "Scene.h"
#include <stdexcept>

void dae::SceneManager::Update(float const deltaTime)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::FixedUpdate(float const fixedTimeStep)
{
	for(auto& scene : m_scenes)
	{
		scene->FixedUpdate(fixedTimeStep);
	}
}

void dae::SceneManager::LateUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}
void dae::SceneManager::RenderImGui()
{
	for (const auto& scene : m_scenes)
	{
		scene->RenderImGui();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&name](const std::shared_ptr<Scene>& scene) {
		return scene->GetName() == name;
		});

	if (it != m_scenes.end())
	{
		m_scenes.erase(it);
	}

}

std::shared_ptr<dae::Scene> dae::SceneManager::GetScene(const std::string& name) const
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
			return scene;
	}

	return nullptr;
}

void dae::SceneManager::SetSceneActive(const std::string& name, bool shouldBeActive)
{
	auto targetScene = GetScene(name);

	if (targetScene)
	{
		targetScene->m_IsActive = shouldBeActive;
	}
	else
	{
		throw std::runtime_error("Scene not found: " + name);
	}
}