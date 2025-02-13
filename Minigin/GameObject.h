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
		virtual void Update(float const deltaTime);
		virtual void FixedUpdate(float const fixedTimeStep);
		virtual void Render() const;

		template<std::derived_from<Component> T, class... Arguments>
		T& AddComponent(Arguments&&... arguments)
		{
			if (HasComponent<T>())
			{
				throw std::runtime_error("Component already exists");
			}

			std::shared_ptr<T> newComponent{ std::make_shared<T>(std::forward<Arguments>(arguments)...) };
			if constexpr (std::is_base_of_v<RenderComponent, T>)
			{
				m_pRenderComponents.emplace_back(newComponent);
			}
			else if constexpr (std::is_base_of_v<PhysicsComponent, T>)
			{
				m_pPhysicsComponents.emplace_back(newComponent);
			}
			else
			{
				m_pMiscComponents.emplace_back(newComponent);
			}

			return *newComponent;
		}

		template<std::derived_from<Component> T>
		void RemoveComponent()
		{
			if constexpr (std::is_base_of_v<RenderComponent, T>)
			{
				for (auto& component : m_pRenderComponents)
				{
					if (dynamic_cast<T*>(component.get()))
					{
						component->SetShouldBeRemoved();
						return;
					}
				}
			}
			else if constexpr (std::is_base_of_v<PhysicsComponent, T>)
			{
				for (auto& component : m_pPhysicsComponents)
				{
					if (dynamic_cast<T*>(component.get()))
					{
						component->SetShouldBeRemoved();
						return;
					}
				}
			}
			else
			{
				for (auto& component : m_pMiscComponents)
				{
					if (dynamic_cast<T*>(component.get()))
					{
						component->SetShouldBeRemoved();
						return;
					}
				}
			}
		}

		template<std::derived_from<Component> T>
		std::optional<T*> GetComponent() const
		{
			if constexpr (std::is_base_of_v<RenderComponent, T>)
			{
				for (const auto& component : m_pRenderComponents)
				{
					if (auto derivedComponent = dynamic_cast<T*>(component.get()))
					{
						return derivedComponent;
					}
				}
			}
			else if constexpr (std::is_base_of_v<PhysicsComponent, T>)
			{
				for (const auto& component : m_pPhysicsComponents)
				{
					if (auto derivedComponent = dynamic_cast<T*>(component.get()))
					{
						return derivedComponent;
					}
				}
			}
			else
			{
				for (const auto& component : m_pMiscComponents)
				{
					if (auto derivedComponent = dynamic_cast<T*>(component.get()))
					{
						return derivedComponent;
					}
				}
			}
			return std::nullopt;
		}

		template<std::derived_from<Component> T>
		bool HasComponent() const
		{
			if constexpr (std::is_base_of_v<RenderComponent, T>)
			{
				for (const auto& component : m_pRenderComponents)
				{
					if (dynamic_cast<T*>(component.get()))
					{
						return true;
					}
				}
			}
			else if constexpr (std::is_base_of_v<PhysicsComponent, T>)
			{
				for (const auto& component : m_pPhysicsComponents)
				{
					if (dynamic_cast<T*>(component.get()))
					{
						return true;
					}
				}
			}
			else
			{
				for (const auto& component : m_pMiscComponents)
				{
					if (dynamic_cast<T*>(component.get()))
					{
						return true;
					}
				}
			}
			return false;
		}

		void SetPosition(float x, float y);
		const Transform& GetPosition() const { return m_Transform; }

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::shared_ptr<RenderComponent>> m_pRenderComponents{};
		std::vector<std::shared_ptr<PhysicsComponent>> m_pPhysicsComponents{};
		std::vector<std::shared_ptr<Component>> m_pMiscComponents{};
		Transform m_Transform{};
	};
}

#endif