#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"


dae::GameObject::GameObject(std::string name, int inputDeviceId)
	: m_Name(name)
	, m_InputDeviceID(inputDeviceId)
{
	m_Transform = std::make_shared<Transform>(*this, 0.0f, 0.0f, 0.0f);
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::SetParentObject(GameObject* parentObject, bool keepWorldPos)
{
	//check if new parent is valid
	if (parentObject == this or IsChild(parentObject)) return;

	//update position
	if (m_pParentObject == nullptr)
	{
		m_Transform->SetLocalPosition(m_Transform->GetGlobalPosition());
	}
	else
	{
		if (keepWorldPos)
		{
			auto parentPos = m_pParentObject->GetTransform()->GetGlobalPosition();
			m_Transform->SetLocalPosition(m_Transform->GetGlobalPosition() - parentPos);
		}
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

void dae::GameObject::SetChildTransformsDirty()
{
	for (auto& child : m_pChildObjects)
	{
		child->GetTransform()->SetDirty();
	}
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