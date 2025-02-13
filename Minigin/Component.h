#ifndef COMPONENT
#define COMPONENT

//#include "GameObject.h"

namespace dae
{
	enum class ComponentType
	{
		RenderComponent,
		PhysicsComponent,
		MiscComponent
	};

	class GameObject;

	class Component
	{
	public:
		virtual void Update(float const deltaTime) = 0;

		Component(dae::GameObject* ownerObject, ComponentType compnentType = ComponentType::MiscComponent);

		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator= (const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		void SetShouldBeRemoved() { m_ShouldBeRemoved = true; }
		bool GetSouldBeRemoved() const { return m_ShouldBeRemoved; }

		ComponentType GetComponentType() const { return m_ComponentType; }

	private:
		GameObject* m_pOwnerObject{};
		ComponentType const m_ComponentType;

	protected:
		GameObject* GetOwner() const { return m_pOwnerObject; }
		bool m_ShouldBeRemoved{ false };
	};
}

#endif