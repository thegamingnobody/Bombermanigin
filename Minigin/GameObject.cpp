#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject(std::string name)
	: m_Name(name)
{
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float const deltaTime)
{
	for (auto& component : m_pRenderComponents)
	{
		component->Update(deltaTime);
	}
	for (auto& component : m_pMiscComponents)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(float const fixedTimeStep)
{
	for (auto& component : m_pPhysicsComponents)
	{
		component->FixedUpdate(fixedTimeStep);
	}	
}

void dae::GameObject::Render() const
{
	for (auto& component : m_pRenderComponents)
	{
		component->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}