#ifndef COMPONENT
#define COMPONENT

//#include "GameObject.h"

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual void Update(float const /*deltaTime*/) {};
		virtual void FixedUpdate(float const /*fixedTimeStep*/) {};
		virtual void Render() const {};
		virtual void RenderImGui() {};

		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator= (const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		void SetShouldBeRemoved() { m_ShouldBeRemoved = true; }
		bool GetSouldBeRemoved() const { return m_ShouldBeRemoved; }

	private:
		GameObject* m_pOwnerObject{};

	protected:
		explicit Component(dae::GameObject* ownerObject);

		GameObject* GetOwner() const { return m_pOwnerObject; }
		bool m_ShouldBeRemoved{ false };
	};
}

#endif