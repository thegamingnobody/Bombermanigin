#ifndef PHYSICSCOMPONENT
#define PHYSICSCOMPONENT

#include "Component.h"

namespace dae
{
	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent(dae::GameObject& ownerObject);

		virtual ~PhysicsComponent() = default;
		PhysicsComponent(const PhysicsComponent& other) = delete;
		PhysicsComponent(PhysicsComponent&& other) = delete;
		PhysicsComponent& operator= (const PhysicsComponent& other) = delete;
		PhysicsComponent& operator=(PhysicsComponent&& other) = delete;

		virtual void FixedUpdate(float const /*fixedTimeStep*/) {};
		virtual void Update(float const /*deltaTime*/) {};

	private:
	};
}

#endif