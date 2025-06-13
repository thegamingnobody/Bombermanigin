#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

std::vector<std::shared_ptr<GameObject>>::iterator Scene::Remove(std::shared_ptr<GameObject> object)
{
	return m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	SetRemoveAll();

	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [](const std::shared_ptr<GameObject>& object)
		{
			return object->GetSouldBeRemoved();

		}), m_objects.end());

}

void dae::Scene::SetRemoveAll()
{
	if (m_objects.size() <= 0) return;

	for (auto& object : m_objects)
	{
		object->SetShouldBeRemoved();
	}
}

void Scene::Update(float const deltaTime)
{
	if (!m_IsActive) return;

	for(auto& object : m_objects)
	{
		if (object->IsHidden()) continue;

		object->Update(deltaTime);
	}

	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [](const std::shared_ptr<GameObject>& object)
		{
			return object->GetSouldBeRemoved();

		}), m_objects.end());

	//for (auto& object : m_objects)
	//{
	//	if (object->GetSouldBeRemoved())
	//	{
	//		Remove(object);
	//	}
	//}
}

void Scene::FixedUpdate(float const fixedTimeStep)
{
	if (!m_IsActive) return;

	for(auto& object : m_objects)
	{
		if (object->IsHidden()) continue;

		object->FixedUpdate(fixedTimeStep);
	}
}

void dae::Scene::LateUpdate()
{
	if (!m_IsActive) return;

	for (auto& object : m_objects)
	{
		if (object->IsHidden()) continue;

		object->LateUpdate();
	}
}

void Scene::Render() const
{
	if (!m_IsActive) return;

	for (const auto& object : m_objects)
	{
		if (object->IsHidden()) continue;

		object->Render();
	}
}

void Scene::RenderImGui() const
{
	if (!m_IsActive) return;

	for (const auto& object : m_objects)
	{
		if (object->IsHidden()) continue;

		object->RenderImGui();
	}
}

std::shared_ptr<GameObject> dae::Scene::GetObject(const std::string& name) const
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [&name](const std::shared_ptr<GameObject>& object)
		{
			return (object->GetName().find(name) != std::string::npos);
		});

	if (it != m_objects.end())
	{
		return *it;
	}
	else
	{
		return nullptr;
	}
}