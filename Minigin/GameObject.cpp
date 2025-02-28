#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"


dae::GameObject::GameObject(std::string name)
	: m_Name(name)
{
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::SetParentObject(GameObject* parentObject, bool keepWorldPos)
{
	//check if new parent is valid
	if (parentObject == nullptr or parentObject == this) return;

	if (IsChild(parentObject)) return;

	//todo: update position
	if (m_pParentObject == nullptr)
	{
		SetLocalPosition(GetGlobalPosition());
	}
	else
	{
		if (keepWorldPos)
		{
			auto parentPos = m_pParentObject->GetGlobalPosition();
			SetLocalPosition(GetGlobalPosition() - parentPos);
		}

		//todo: set position dirty
	}

	//remove child from old parent
	if (m_pParentObject)
	{
		auto it = std::find(m_pParentObject->m_pChildObjects.begin(), m_pParentObject->m_pChildObjects.end(), this);

		if (it != m_pParentObject->m_pChildObjects.end())
		{
			m_pParentObject->m_pChildObjects.erase(it);
		}
	}

	//set new parent
	m_pParentObject = parentObject;

	//add to parent as child
	m_pParentObject->m_pChildObjects.emplace_back(this);

}

bool dae::GameObject::IsChild(GameObject* object)
{
	for (auto& child : m_pChildObjects)
	{
		if (child == object)
		{
			return true;
		}
	}
	return false;
}

void dae::GameObject::Update(float const deltaTime)
{
	for (auto& component : m_pComponents)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(float const fixedTimeStep)
{
	for (auto& component : m_pComponents)
	{
		component->FixedUpdate(fixedTimeStep);
	}	
}

void dae::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
		component->Render();
	}
}

void dae::GameObject::RenderImGui() const
{
	for (auto& component : m_pComponents)
	{
		component->RenderImGui();
	}
}

glm::vec3 dae::GameObject::GetGlobalPosition() const
{
	auto transformComp = GetComponent<Transform>();
	if (transformComp.has_value())
	{
		return transformComp.value()->GetGlobalPosition();
	}
	else
	{
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

glm::vec3 dae::GameObject::GetLocalPosition() const
{
	auto transformComp = GetComponent<Transform>();
	if (transformComp.has_value())
	{
		return transformComp.value()->GetLocalPosition();
	}
	else
	{
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	auto transformComp = GetComponent<Transform>();
	if (transformComp.has_value())
	{
		transformComp.value()->SetLocalPosition(x, y, 0.0f);
	}
}

void dae::GameObject::SetLocalPosition(glm::vec3 pos)
{
	SetLocalPosition(pos.x, pos.y);
}

void dae::GameObject::SetLocalPosition(glm::vec2 pos)
{
	SetLocalPosition(pos.x, pos.y);
}
