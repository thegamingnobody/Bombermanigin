#ifndef ROTATORCOMPONENT
#define ROTATORCOMPONENT

#include "PhysicsComponent.h"
#include <glm.hpp>
#include "GameObject.h"

namespace dae
{
    class RotatorComponent : public PhysicsComponent
    {
    public:
        RotatorComponent(GameObject* ownerObject, float speed = 1.0f, float radius = 10.0f);

        virtual ~RotatorComponent() = default;
        RotatorComponent(const RotatorComponent& other) = delete;
        RotatorComponent(RotatorComponent&& other) = delete;
        RotatorComponent& operator= (const RotatorComponent& other) = delete;
        RotatorComponent& operator=(RotatorComponent&& other) = delete;

        virtual void FixedUpdate(float const fixedTimeStep) override;
        virtual void Update(float const deltaTime) override;

    private:
		float m_Speed;
        float m_CurrentAngle;
        float m_Radius;
		glm::vec2 m_Center;
		glm::vec2 m_CurrentOffset;
    };
}

#endif