#pragma once

#include <glm.hpp>
#include "Component.h"
#include <optional>
#include "Transform.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		//*-----------------------------------------*
		//|					Rule of 5				|
		//*-----------------------------------------*
		GameObject(std::string name = "", glm::vec3 startingPos = glm::vec3(0.0f, 0.0f, 0.0f));
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		
		//*-----------------------------------------*
		//|				Basic Functions				|
		//*-----------------------------------------*
		void Update(float const deltaTime);
		void FixedUpdate(float const fixedTimeStep);
		void LateUpdate();
		void Render() const;
		void RenderImGui() const;

		//*-----------------------------------------*
		//|			 Component Functions			|
		//*-----------------------------------------*
		template<std::derived_from<Component> T, class... Arguments>
		T& AddComponent(Arguments&&... arguments)
		{
			std::shared_ptr<T> newComponent{ std::make_shared<T>(std::forward<Arguments>(arguments)...) };
			m_pComponents.emplace_back(newComponent);

			return *newComponent;
		}

		template<std::derived_from<Component> T>
		void RemoveComponent()
		{
			for (auto& component : m_pComponents)
			{
				if (dynamic_cast<T*>(component.get()))
				{
					component->SetShouldBeRemoved();
					return;
				}
			}
		}

		template<std::derived_from<Component> T>
		std::optional<T*> GetComponent() const
		{
			for (const auto& component : m_pComponents)
			{
				if (auto derivedComponent = dynamic_cast<T*>(component.get()))
				{
					return derivedComponent;
				}
			}
			return std::nullopt;
		}

		template<std::derived_from<Component> T>
		bool HasComponent() const
		{
			for (const auto& component : m_pComponents)
			{
				if (dynamic_cast<T*>(component.get()))
				{
					return true;
				}
			}
			return false;
		}

		//*-----------------------------------------*
		//|			  Transform functions			|
		//*-----------------------------------------*
		Transform* GetTransform() { return m_Transform.get(); }
		void SetChildTransformsDirty();

		//*-----------------------------------------*
		//|			Parent/Child Functions			|
		//*-----------------------------------------*
		void SetParentObject(GameObject* parentObject, bool keepWorldPos);
		GameObject* GetParentObject() const { return m_pParentObject; }

		int GetChildCount() const { return static_cast<int>(m_pChildObjects.size()); }
		GameObject* GetChildObject(int index) const { return m_pChildObjects[index]; }

		void SetShouldBeRemoved() { m_ShouldBeRemoved = true; }
		bool GetSouldBeRemoved() const { return m_ShouldBeRemoved; }

		std::string GetName() const { return m_Name; }

		void SetIsHidden(bool isHidden) { m_IsHidden = isHidden; }
		bool IsHidden() const { return m_IsHidden; }

		void Rename(const std::string& newName);

	private:
		bool IsChild(GameObject* object);

		std::string m_Name;

		std::vector<std::shared_ptr<Component>> m_pComponents{};

		std::shared_ptr<Transform> m_Transform;

		GameObject* m_pParentObject{};
		std::vector<GameObject*> m_pChildObjects{};

		bool m_ShouldBeRemoved{ false };

		bool m_IsHidden{ false };
	};
}