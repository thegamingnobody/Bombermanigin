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
		//*-----------------------------------------*
		//|					Rule of 5				|
		//*-----------------------------------------*
		GameObject(std::string name = "", int inputDeviceId = -1);
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
		void Render() const;
		void RenderImGui() const;

		//*-----------------------------------------*
		//|			 Component Functions			|
		//*-----------------------------------------*
		template<std::derived_from<Component> T, class... Arguments>
		T& AddComponent(Arguments&&... arguments)
		{
			if (HasComponent<T>())
			{
				throw std::runtime_error("Component already exists");
			}

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
	private:
		bool IsChild(GameObject* object);
		void SetChildTransformsDirty();

		std::string m_Name;

		std::vector<std::shared_ptr<Component>> m_pComponents{};

		std::shared_ptr<Transform> m_Transform;

		GameObject* m_pParentObject{};
		std::vector<GameObject*> m_pChildObjects{};

		int m_InputDeviceID;

		bool m_ShouldBeRemoved{ false };
	};
}

#endif