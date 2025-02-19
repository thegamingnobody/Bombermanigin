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

void dae::GameObject::AddChildObject(GameObject* childObject)
{
	if (childObject == nullptr or childObject == this) return;
	
	//todo: update position
	//childObject->SetPosition(m_Transform.GetPosition().x, m_Transform.GetPosition().y);

	//remove child from old parent
	if (childObject)
	{
		auto it = std::find(childObject->m_pParentObject->m_pChildObjects.begin(), childObject->m_pParentObject->m_pChildObjects.end(), childObject);
		if (it != childObject->m_pParentObject->m_pChildObjects.end())
		{
			RemoveChildObject(childObject);
		}
	}

	//set new parent
	childObject->m_pParentObject = this;

	//add to parent as child
	m_pChildObjects.emplace_back(childObject);
}

void dae::GameObject::RemoveChildObject(GameObject* childObject)
{
	if (childObject == nullptr or childObject == this) return;

	auto it = std::find(m_pChildObjects.begin(), m_pChildObjects.end(), childObject);
	if (it != m_pChildObjects.end())
	{
		m_pChildObjects.erase(it);
	}

	childObject->m_pParentObject = nullptr;

	//todo: update position
}

void dae::GameObject::Update(float const deltaTime)
{
	//for (auto& component : m_pRenderComponents)
	//{
	//	component->Update(deltaTime);
	//}
	for (auto& component : m_pComponents)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(float const fixedTimeStep)
{
	for (auto& component : m_pComponents)
	{
		if (auto derivedComponent = dynamic_cast<dae::PhysicsComponent*>(component.get()))
		{
			derivedComponent->FixedUpdate(fixedTimeStep);
		}
	}	
}

void dae::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
		if (auto derivedComponent = dynamic_cast<dae::RenderComponent*>(component.get()))
		{
			derivedComponent->Render();
		}
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
