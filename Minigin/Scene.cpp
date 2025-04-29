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

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float const deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}

	for (auto& object : m_objects)
	{
		if (object->GetSouldBeRemoved())
		{
			Remove(object);
		}
	}
}

void Scene::FixedUpdate(float const fixedTimeStep)
{
	for(auto& object : m_objects)
	{
		object->FixedUpdate(fixedTimeStep);
	}
}

void dae::Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void Scene::RenderImGui() const
{
	for (const auto& object : m_objects)
	{
		object->RenderImGui();
	}
}

std::shared_ptr<GameObject> dae::Scene::GetObject(const std::string& name) const
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [&name](const std::shared_ptr<GameObject>& object)
		{
			return object->GetName() == name;
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