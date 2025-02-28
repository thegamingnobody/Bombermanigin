#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <memory>
#include "Transform.h"
#include <string>
#include <vector>
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "Component.h"
#include <iostream>
#include <optional>

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		void Update(float const deltaTime);
		void FixedUpdate(float const fixedTimeStep);
		void Render() const;
		void RenderImGui() const;

		template<std::derived_from<Component> T, class... Arguments>
		T& AddComponent(Arguments&&... arguments)
		{
			if (HasComponent<T>())
			{
				throw std::runtime_error("Component already exists");
			}

			std::shared_ptr<T> newComponent{ std::make_shared<T>(std::forward<Arguments>(arguments)...) };
			//if constexpr (std::is_base_of_v<RenderComponent, T>)
			//{
			//	m_pRenderComponents.emplace_back(newComponent);
			//}
			//else if constexpr (std::is_base_of_v<PhysicsComponent, T>)
			//{
			//	m_pPhysicsComponents.emplace_back(newComponent);
			//}
			//else
			//{
				m_pComponents.emplace_back(newComponent);
			//}

			return *newComponent;
		}

		template<std::derived_from<Component> T>
		void RemoveComponent()
		{
			//if constexpr (std::is_base_of_v<RenderComponent, T>)
			//{
			//	for (auto& component : m_pRenderComponents)
			//	{
			//		if (dynamic_cast<T*>(component.get()))
			//		{
			//			component->SetShouldBeRemoved();
			//			return;
			//		}
			//	}
			//}
			//else if constexpr (std::is_base_of_v<PhysicsComponent, T>)
			//{
			//	for (auto& component : m_pPhysicsComponents)
			//	{
			//		if (dynamic_cast<T*>(component.get()))
			//		{
			//			component->SetShouldBeRemoved();
			//			return;
			//		}
			//	}
			//}
			//else
			//{
				for (auto& component : m_pComponents)
				{
					if (dynamic_cast<T*>(component.get()))
					{
						component->SetShouldBeRemoved();
						return;
					}
				}
			//}
		}

		template<std::derived_from<Component> T>
		std::optional<T*> GetComponent() const
		{
			//if constexpr (std::is_base_of_v<RenderComponent, T>)
			//{
			//	for (const auto& component : m_pRenderComponents)
			//	{
			//		if (auto derivedComponent = dynamic_cast<T*>(component.get()))
			//		{
			//			return derivedComponent;
			//		}
			//	}
			//}
			//else if constexpr (std::is_base_of_v<PhysicsComponent, T>)
			//{
			//	for (const auto& component : m_pPhysicsComponents)
			//	{
			//		if (auto derivedComponent = dynamic_cast<T*>(component.get()))
			//		{
			//			return derivedComponent;
			//		}
			//	}
			//}
			//else
			//{
				for (const auto& component : m_pComponents)
				{
					if (auto derivedComponent = dynamic_cast<T*>(component.get()))
					{
						return derivedComponent;
					}
				}
			//}
			return std::nullopt;
		}

		template<std::derived_from<Component> T>
		bool HasComponent() const
		{
			//if constexpr (std::is_base_of_v<RenderComponent, T>)
			//{
			//	for (const auto& component : m_pRenderComponents)
			//	{
			//		if (dynamic_cast<T*>(component.get()))
			//		{
			//			return true;
			//		}
			//	}
			//}
			//else if constexpr (std::is_base_of_v<PhysicsComponent, T>)
			//{
			//	for (const auto& component : m_pPhysicsComponents)
			//	{
			//		if (dynamic_cast<T*>(component.get()))
			//		{
			//			return true;
			//		}
			//	}
			//}
			//else
			//{
				for (const auto& component : m_pComponents)
				{
					if (dynamic_cast<T*>(component.get()))
					{
						return true;
					}
				}
			//}
			return false;
		}

		glm::vec3 GetGlobalPosition() const;
		glm::vec3 GetLocalPosition() const;
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(glm::vec3 pos);
		void SetLocalPosition(glm::vec2 pos);

		GameObject(std::string name = "");
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void SetParentObject(GameObject* parentObject, bool keepWorldPos);
		GameObject* GetParentObject() const { return m_pParentObject; }

		int GetChildCount() const { return static_cast<int>(m_pChildObjects.size()); }
		GameObject* GetChildObject(int index) const { return m_pChildObjects[index]; }

		void SetShouldBeRemoved() { m_ShouldBeRemoved = true; }
		bool GetSouldBeRemoved() const { return m_ShouldBeRemoved; }

	private:
		bool IsChild(GameObject* object);

		std::string m_Name;

		GameObject* m_pParentObject{};
		std::vector<GameObject*> m_pChildObjects{};

		//std::vector<std::shared_ptr<RenderComponent>> m_pRenderComponents{};
		//std::vector<std::shared_ptr<PhysicsComponent>> m_pPhysicsComponents{};
		std::vector<std::shared_ptr<Component>> m_pComponents{};

		//Transform m_Transform{};

		bool m_ShouldBeRemoved{ false };
	};
}

#endif